/*!
 * \defgroup grpVT100 VT100
 * @{
 */
// VT100 : ANSI escape code
#define CSI             "\x1B["             /*!< Séquence escape ANSI */
#if defined(_MSDOS) || defined(_WIN32) || defined(__CYGWIN32__) || defined(__CYGWIN__)
    #define CRLF        "\n\r"              /*!< Retour à la ligne (WINDOWS) */
#else
    #define CRLF        "\n"                /*!< Retour à la ligne (LINUX) */
#endif

/*!
 * \defgroup actecrVT100 Actions sur l'écran d'un terminal
 * @{
 */
// VT100 : Actions sur l'écran d'un terminal
#define CLEAREOL        CSI "K"             /*!< Clear End Of Line */
#define CLEARLCR        CSI "0K"            /*!< Clear Line Cursor Right */
#define CLEARLCL        CSI "1K"            /*!< Clear Line Cursor Left */
#define CLEARELN        CSI "2K"            /*!< Clear Entire LiNe */
#define CLEAREOS        CSI "J"             /*!< Clear End Of Screen */
#define CLEARCDW        CSI "0J"            /*!< Clear Curseur DoWn */
#define CLEARCUP        CSI "1J"            /*!< Clear Curseur UP */
#define CLEARSCR        CSI "2J" CSI ";H"   /*!< Clear SCReen */
/*!
 * @}
 */

/*!
 * \defgroup actcurVT100 Actions sur le curseur
 * @{
 */
// VT100 : Actions sur le curseur
#define HIDECURSOR      CSI "?25l"          /*!< Fait disparaitre le curseur */
#define SHOWCURSOR      CSI "?25h"          /*!< Affiche le curseur à l'écran */
#define GOTOYX          CSI "%.2d;%.2dH"    /*!< Positionne le curseur en (y,x) */
#define CURSORFORWARD   CSI "%dC"           /*!< Avance le curseur de %d caractères */
#define CURSORBACKWARD  CSI "%dD"           /*!< Recule le curseur de %d caractères */
#define SAVCURSORPOS    CSI "s"             /*!< Sauve la position du curseur */
#define RESCURSORPOS    CSI "u"             /*!< Restore la position du curseur */
#define INSERTMOD       CSI "4h"            /*!< Mode insertion */
#define OVERWRITEMOD    CSI "4l"            /*!< Mode de non insertion */
#define DECSTBM         CSI "%.2d;%.2dr"    /*!< Positionne la marge haute et basse */
/*!
 * @}
 */

/*!
 * \defgroup actcarVT100 Actions sur les caractères affichables
 * @{
 */
// VT100 : Actions sur les caractères affichables
#define CA_RESET        CSI "00m"           /*!< Reset */
#define CA_BRIGHT       CSI "01m"           /*!< Gras */
#define CA_NOBRIGHT     CSI "22m"           /*!< Normal */
#define CA_UNDERLINE    CSI "04m"           /*!< Souligné */
#define CA_NOUNDERLINE  CSI "24m"           /*!< Non souligné */
#define CA_BLINKING     CSI "05m"           /*!< Clignotant */
#define CA_NOBLINKING   CSI "25m"           /*!< Non clignotant */
#define CA_REVERSE      CSI "07m"           /*!< Inverse vidéo */
#define CA_NOREVERSE    CSI "27m"           /*!< Vidéo non inversée */
#define CA_HIDDEN       CSI "08m"           /*!< Invisible */
#define CA_NOHIDDEN     CSI "28m"           /*!< Visible */
/*!
 * @}
 */

/*!
 * \defgroup actcoulVT100 Couleurs du texte
 * @{
 */
// VT100 : Couleurs du texte
#define CL_BLACK        CSI "22;30m"        /*!< Noir */
#define CL_RED          CSI "22;31m"        /*!< Rouge */
#define CL_GREEN        CSI "22;32m"        /*!< Vert */
#define CL_BROWN        CSI "22;33m"        /*!< Brun */
#define CL_BLUE         CSI "22;34m"        /*!< Bleu */
#define CL_MAGENTA      CSI "22;35m"        /*!< Magenta */
#define CL_CYAN         CSI "22;36m"        /*!< Cyan */
#define CL_LIGHTGRAY    CSI "22;37m"        /*!< Gris */
#define CL_DARKGRAY     CSI "01;30m"        /*!< Gris foncé */
#define CL_LIGHTRED     CSI "01;31m"        /*!< Rouge clair */
#define CL_LIGHTGREEN   CSI "01;32m"        /*!< Vert clair */
#define CL_YELLOW       CSI "01;33m"        /*!< Jaune */
#define CL_LIGHTBLUE    CSI "01;34m"        /*!< Bleu clair */
#define CL_LIGHTMAGENTA CSI "01;35m"        /*!< Magenta clair */
#define CL_LIGHTCYAN    CSI "01;36m"        /*!< Cyan clair */
#define CL_WHITE        CSI "01;37m"        /*!< Blanc */
/*!
 * @}
 */

/*!
 * \defgroup actcoulfondVT100 Couleurs du fond
 * @{
 */
// VT100 : Couleurs de fond
#define BK_BLACK        CSI "22;40m"        /*!< Noir */
#define BK_RED          CSI "22;41m"        /*!< Rouge */
#define BK_GREEN        CSI "22;42m"        /*!< Vert */
#define BK_YELLOW       CSI "22;43m"        /*!< Jaune */
#define BK_BLUE         CSI "22;44m"        /*!< Bleu */
#define BK_MAGENTA      CSI "22;45m"        /*!< Magenta */
#define BK_CYAN         CSI "22;46m"        /*!< Cyan */
#define BK_WHITE        CSI "22;47m"        /*!< Blanc */
/*!
 * @}
 */

/*!
 * \defgroup deftouchVT100 Quelques touches mono-code
 * @{
 */
// VT100 : Quelques touches mono-code
#define V_DEL           0x08                /*!< Touche DEL */
#define V_TAB           0x09                /*!< Touche TABulation */
#define V_ENTER         0x0D                /*!< Touche ENTER */
#define V_ESC           0x1B                /*!< Touche ESCape */

#define V_CTRLA         0x01                /*!< Touche CTRL+A */
#define V_CTRLB         0x02                /*!< Touche CTRL+B */
#define V_CTRLC         0x03                /*!< Touche CTRL+C */
#define V_CTRLD         0x04                /*!< Touche CTRL+D */
#define V_CTRLE         0x05                /*!< Touche CTRL+E */
#define V_CTRLF         0x06                /*!< Touche CTRL+F */
#define V_CTRLG         0x07                /*!< Touche CTRL+G */
#define V_CTRLH         0x08                /*!< Touche CTRL+H */
#define V_CTRLI         0x09                /*!< Touche CTRL+I */
#define V_CTRLJ         0x0A                /*!< Touche CTRL+J */
#define V_CTRLK         0x0B                /*!< Touche CTRL+K */
#define V_CTRLL         0x0C                /*!< Touche CTRL+L */
#define V_CTRLM         0x0D                /*!< Touche CTRL+M - Attention: identique à V_ENTER */
#define V_CTRLN         0x0E                /*!< Touche CTRL+N */
#define V_CTRLO         0x0F                /*!< Touche CTRL+O */
#define V_CTRLP         0x10                /*!< Touche CTRL+P */
#define V_CTRLQ         0x11                /*!< Touche CTRL+Q */
#define V_CTRLR         0x12                /*!< Touche CTRL+R */
#define V_CTRLS         0x13                /*!< Touche CTRL+S */
#define V_CTRLT         0x14                /*!< Touche CTRL+T */
#define V_CTRLU         0x15                /*!< Touche CTRL+U */
#define V_CTRLV         0x16                /*!< Touche CTRL+V */
#define V_CTRLW         0x17                /*!< Touche CTRL+W */
#define V_CTRLX         0x18                /*!< Touche CTRL+X */
#define V_CTRLY         0x19                /*!< Touche CTRL+Y */
#define V_CTRLZ         0x1A                /*!< Touche CTRL+Z */
/*!
 * @}
 */

/*!
 * \defgroup def3touchVT100 Quelques touches triple-code
 * @{
 */
// VT100 : Quelques touches triple-code
/*
    Structure d'une touche : ESC<code1><code2>
    Exp : Flêche Haute = ESC[A  ==> V_ESC + VCD1 + V_FLCH
    Exp : Touche F1    = ESCOA  ==> V_ESC + VCD2 + V_F1
*/
#define VCD1    '['             /*!< Code 1 */
  #define V_FLCH        'A'     /*!< Flêche haute */
  #define V_FLCB        'B'     /*!< Flêche basse */
  #define V_FLCD        'C'     /*!< Flêche droite */
  #define V_FLCG        'D'     /*!< Flêche gauche */

#define VCD2    'O'             /*!< Code 2 */
  #define V_F1          'P'     /*!< Touche F1 */
  #define V_F2          'Q'     /*!< Touche F2 */
  #define V_F3          'R'     /*!< Touche F3 */
  #define V_F4          'S'     /*!< Touche F4 */
  #define V_F5          'T'     /*!< Touche F5 */
  #define V_F6          'U'     /*!< Touche F6 */
  #define V_F7          'V'     /*!< Touche F7 */
  #define V_F8          'W'     /*!< Touche F8 */
  #define V_F9          'X'     /*!< Touche F9 */
  #define V_F10         'Y'     /*!< Touche F10 */
  #define V_F11         'Z'     /*!< Touche F11 */
  #define V_F12         'A'     /*!< Touche F12 */
/*!
 * @}
 * @}
 */