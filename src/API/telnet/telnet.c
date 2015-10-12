/**
 *
 * \file
 * Telnet protocol
 *
 */

/*
 *
 * Copyright (c) 2010, Jesper Hansen
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *      * Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holders nor the names of any contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */


#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"

//#include "utils/uartstdio.h"
//#include "utils/ustdlib.h"

#include <string.h>
#include "telnet.h"

#define usprintf 	_sprintf
#define UARTprintf	_printf

#define TELNET_IAC   255
#define TELNET_WILL  251
#define TELNET_WONT  252
#define TELNET_DO    253
#define TELNET_DONT  254

#define IDLE_TIMEOUT	30		// 30 seconds

struct telnet_state {
	char		*data_out;
	char		cmd_buffer[80];
	u16_t		left;
	u16_t		timeout;
	u16_t		flags;
#define TELNET_FLAG_CLOSE_CONNECTION	(1<<0)
	u16_t		state;
#define STATE_NORMAL 0
#define STATE_IAC    1
#define STATE_WILL   2
#define STATE_WONT   3
#define STATE_DO     4
#define STATE_DONT   5
#define STATE_CLOSE  6
};


char tstr[2000];
char *prompt = "\r\nJdTelnet>";
extern char VERSION_SW[];

void addPrompt(struct telnet_state *hs)
{
	usprintf(&hs->data_out[hs->left], "%s", prompt);
	hs->left += strlen(prompt);
}

// telnet command handler
void cmd_parser(struct telnet_state *hs)
{
	char* p = hs->cmd_buffer;
	char* pDeb;
	char* pParam0;

	if (strstr(p, "quit"))
	{
		hs->left = 0;
		hs->flags |= TELNET_FLAG_CLOSE_CONNECTION;
	}
	else if (pDeb = strstr(p, "ls"))
	{
		if (strlen(pDeb) > 3)
			MemoireFAT_PrintFileList(pDeb + 3, tstr, 2000);
		else
			MemoireFAT_PrintFileList("", tstr, 2000);
		hs->data_out = tstr;
		hs->left = strlen(hs->data_out);
		addPrompt(hs);
	}
	else if (pDeb = strstr(p, "get_v"))
	{
		usprintf(tstr,"VersionSW = AL%s\r\n", VERSION_SW);
		hs->data_out = tstr;
		hs->left = strlen(hs->data_out);
		addPrompt(hs);
	}
	else if (pDeb = strstr(p, "reboot"))
	{
		usprintf(tstr,"Rebooting device...\r\n");
		hs->data_out = tstr;
		hs->left = strlen(hs->data_out);
		addPrompt(hs);

		GOTO(0);
	}
	else if (pDeb = strstr(p, "delete "))
	{
		pParam0 = pDeb + strlen("delete ");
		f_unlink(pParam0);
		usprintf(tstr,"Delete file: %s\r\n", pParam0);
		hs->data_out = tstr;
		hs->left = strlen(hs->data_out);
		addPrompt(hs);
	}
	else
	{
		usprintf(tstr,"\r\nThis is TELNET echoing your command : \"%s\"%s",p,prompt);
		hs->data_out = tstr;
		hs->left = strlen(hs->data_out);
	}

	hs->cmd_buffer[0] = 0;
	// copy any remaining part of command line to position 0
	//strcpy(hs->cmd_buffer,q+1);
}




/*-----------------------------------------------------------------------------------*/
static void conn_err(void *arg, err_t err)
{
	struct http_state *hs;
	LWIP_UNUSED_ARG(err);
	hs = arg;
	mem_free(hs);
}

/*-----------------------------------------------------------------------------------*/
static void close_conn(struct tcp_pcb *pcb, struct telnet_state *hs)
{
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	mem_free(hs);
	tcp_close(pcb);
}

/*-----------------------------------------------------------------------------------*/
static void send_data(struct tcp_pcb *pcb, struct telnet_state *hs)
{
	err_t err;
	u16_t len;

	/* We cannot send more data than space available in the send
	buffer. */
	if (tcp_sndbuf(pcb) < hs->left) {
		len = tcp_sndbuf(pcb);
	} else {
		len = hs->left;
	}

	do {
		err = tcp_write(pcb, hs->data_out, len, 0);
		if (err == ERR_MEM) {
			len /= 2;
		}
	} while (err == ERR_MEM && len > 1);

	if (err == ERR_OK) {
		hs->data_out += len;
		hs->left -= len;
	}
}

/*-----------------------------------------------------------------------------------*/
static err_t telnet_poll(void *arg, struct tcp_pcb *pcb)
{
	struct telnet_state *hs;

	hs = arg;

	UARTprintf("Poll\n");

	if (hs == NULL) {
		tcp_abort(pcb);
		return ERR_ABRT;
	} else {

		if (hs->flags & TELNET_FLAG_CLOSE_CONNECTION) {
			UARTprintf("closing connection in poll\n");
			close_conn(pcb, hs);
			hs->flags &= ~TELNET_FLAG_CLOSE_CONNECTION;
		}

		// timeout and close connection if nothing has been received for N seconds
		hs->timeout++;
		if (hs->timeout > IDLE_TIMEOUT)
			close_conn(pcb,hs);
	}
	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
static err_t telnet_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	struct telnet_state *hs;

	LWIP_UNUSED_ARG(len);

	hs = arg;

	if (hs->left > 0) {
		send_data(pcb, hs);
	} else {
		if (hs->flags & TELNET_FLAG_CLOSE_CONNECTION) {
			UARTprintf("closing connection\n");
			close_conn(pcb, hs);
			hs->flags &= ~TELNET_FLAG_CLOSE_CONNECTION;
		}
	}

	return ERR_OK;
}

void sendopt(struct telnet_state *hs, u8_t option, u8_t value)
{
	hs->data_out[hs->left++] = TELNET_IAC;
	hs->data_out[hs->left++] = option;
	hs->data_out[hs->left++] = value;
	hs->data_out[hs->left] = 0;
}

void get_char(struct tcp_pcb *pcb, struct telnet_state *hs, char c)
{
	int l;

	if (c == '\r') return;
	if (c != '\n')
	{
		l = strlen(hs->cmd_buffer);
		hs->cmd_buffer[l++] = c;
		hs->cmd_buffer[l] = 0;
	}
	else
	{
		cmd_parser(hs);		// handle command
		if (hs->left > 0)
		{
			send_data(pcb, hs);
			tcp_sent(pcb, telnet_sent);
		}
	}
}

/*-----------------------------------------------------------------------------------*/
static err_t telnet_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	struct telnet_state *hs;
	char *q,c;
	int len;

	hs = arg;

	if (err == ERR_OK && p != NULL) {

		/* Inform TCP that we have taken the data. */
		tcp_recved(pcb, p->tot_len);
		hs->timeout = 0;	// reset idletime ount

		q = p->payload;
		len = p->tot_len;

		while(len > 0)
		{
			c = *q++;
			--len;

			switch (hs->state) {
				case STATE_IAC:
					if(c == TELNET_IAC) {
						get_char(pcb,hs,c);
						hs->state = STATE_NORMAL;
					} else {
						switch(c) {
							case TELNET_WILL:
								hs->state = STATE_WILL;
								break;
							case TELNET_WONT:
								hs->state = STATE_WONT;
								break;
							case TELNET_DO:
								hs->state = STATE_DO;
								break;
							case TELNET_DONT:
								hs->state = STATE_DONT;
								break;
							default:
								hs->state = STATE_NORMAL;
								break;
						}
					}
					break;

				case STATE_WILL:
					/* Reply with a DONT */
					sendopt(hs,TELNET_DONT, c);
					hs->state = STATE_NORMAL;
					break;
				case STATE_WONT:
					/* Reply with a DONT */
					sendopt(hs,TELNET_DONT, c);
					hs->state = STATE_NORMAL;
					break;
				case STATE_DO:
					/* Reply with a WONT */
					sendopt(hs,TELNET_WONT, c);
					hs->state = STATE_NORMAL;
					break;
				case STATE_DONT:
					/* Reply with a WONT */
					sendopt(hs,TELNET_WONT, c);
					hs->state = STATE_NORMAL;
					break;
				case STATE_NORMAL:
					if(c == TELNET_IAC) {
						hs->state = STATE_IAC;
					} else {
						get_char(pcb,hs,c);
					}
					break;
			}
		}

		//! \todo should check max length here
//		strcat(hs->cmd_buffer,p->payload);

		pbuf_free(p);
/*
		if (strchr(hs->cmd_buffer,'\r'))
		{
			cmd_parser(hs);		// handle command

			send_data(pcb, hs);

			// Tell TCP that we wish be to informed of data that has been
			// successfully sent by a call to the http_sent() function.
			tcp_sent(pcb, telnet_sent);
		}
*/

	}

	if (err == ERR_OK && p == NULL) {
		UARTprintf("remote host closed connection\n");
		close_conn(pcb, hs);
	}

	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
static err_t telnet_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	struct telnet_state *hs;

	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	tcp_setprio(pcb, TCP_PRIO_MIN);

	/* Allocate memory for the structure that holds the state of the
	connection. */
	hs = (struct telnet_state *)mem_malloc(sizeof(struct telnet_state));

	if (hs == NULL) {
		UARTprintf("telnet_accept: Out of memory\n");
		return ERR_MEM;
	}

	UARTprintf("new connection from %d.%d.%d.%d\n", (pcb->remote_ip.addr >> 0) & 0xff, (pcb->remote_ip.addr >> 8) & 0xff, (pcb->remote_ip.addr >> 16) & 0xff, (pcb->remote_ip.addr >> 24) & 0xff);

	/* Initialize the structure. */
	hs->data_out = NULL;
	hs->cmd_buffer[0] = 0;
	hs->left = 0;
	hs->timeout = 0;
	hs->flags = 0;
	hs->state = STATE_NORMAL;

	/* Tell TCP that this is the structure we wish to be passed for our
	callbacks. */
	tcp_arg(pcb, hs);

	/* Tell TCP that we wish to be informed of incoming data by a call
	to the http_recv() function. */
	tcp_recv(pcb, telnet_recv);

	tcp_err(pcb, conn_err);
	tcp_poll(pcb, telnet_poll, 4);

	usprintf(tstr,"\nWelcome to JD TELNET.\r\n%s", prompt);
	hs->data_out = tstr;
	hs->left = strlen(tstr);
	send_data(pcb, hs);

	return ERR_OK;
}

/**
 * Initialize telnet by setting up a raw connection to
 * listen on port 23
 *
 */
void telnet_init(void)
{
	struct tcp_pcb *pcb;

	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, 23);
	pcb = tcp_listen(pcb);
	tcp_accept(pcb, telnet_accept);
}

