melissa@debian:~$ ssh 192.168.7.
^[[A^[[A^C
melissa@debian:~$ ssh 192.168.7.2
Debian GNU/Linux 13

BeagleBoard.org Debian Trixie Xfce Image 2025-08-29
Support: https://bbb.io/debian
default username is [melissa]

          _____                    _____          
         /\    \                  /\    \         
        /::\____\                /::\    \        
       /:::/    /                \:::\    \       
      /:::/    /                  \:::\    \      
     /:::/    /                    \:::\    \     
    /:::/____/                      \:::\    \    
   /::::\    \                      /::::\    \   
  /::::::\    \   _____    ____    /::::::\    \  
 /:::/\:::\    \ /\    \  /\   \  /:::/\:::\    \ 
/:::/  \:::\    /::\____\/::\   \/:::/  \:::\____\
\::/    \:::\  /:::/    /\:::\  /:::/    \::/    /
 \/____/ \:::\/:::/    /  \:::\/:::/    / \/____/ 
          \::::::/    /    \::::::/    /          
           \::::/    /      \::::/____/           
           /:::/    /        \:::\    \           
          /:::/    /          \:::\    \          
         /:::/    /            \:::\    \         
        /:::/    /              \:::\____\        
        \::/    /                \::/    /        
         \/____/                  \/____/         
                                                  
Last login: Sun Oct 26 21:28:31 2025 from 192.168.7.1
[sudo] password for melissa: 
Hello embedded world!, from me!
When the screen prompts you, move the joystick in that direction (up or down)
^Cmelissa@beagle:~gpioinfo
gpiochip0 - 24 lines:
	line   0:	"GPIO8"         	output active-low consumer="spi0 CS0"
	line   1:	unnamed         	input
	line   2:	"GPIO11"        	output consumer="sck"
	line   3:	"GPIO10"        	output consumer="mosi"
	line   4:	"GPIO9"         	input consumer="miso"
	line   5:	unnamed         	input
	line   6:	unnamed         	output consumer=reset
	line   7:	"GPIO23"        	input
	line   8:	unnamed         	output consumer=enable
	line   9:	"GPIO7"         	output active-low consumer="spi0 CS1"
	line  10:	"GPIO24"        	input
	line  11:	unnamed         	input
	line  12:	unnamed         	input
	line  13:	"HDMI_RSTn"     	input
	line  14:	"HDMI_INTn"     	input
	line  15:	unnamed         	input
	line  16:	unnamed         	input
	line  17:	"GPIO3"         	input
	line  18:	"GPIO2"         	input
	line  19:	unnamed         	input
	line  20:	unnamed         	input
	line  21:	unnamed         	input
	line  22:	unnamed         	input
	line  23:	unnamed         	input
gpiochip1 - 87 lines:
	line   0:	unnamed         	input
	line   1:	unnamed         	output consumer=MIPI_SWITCH_OE
	line   2:	unnamed         	output consumer=MIPI_SWITCH_SEL
	line   3:	unnamed         	input
	line   4:	unnamed         	output consumer=regulator-7
	line   5:	unnamed         	input
	line   6:	unnamed         	input
	line   7:	unnamed         	input
	line   8:	unnamed         	input
	line   9:	unnamed         	input
	line  10:	unnamed         	input
	line  11:	unnamed         	output active-low consumer=PWR
	line  12:	unnamed         	output consumer=ACT
	line  13:	unnamed         	input
	line  14:	unnamed         	input
	line  15:	unnamed         	input
	line  16:	unnamed         	input
	line  17:	unnamed         	input
	line  18:	unnamed         	input
	line  19:	unnamed         	input
	line  20:	unnamed         	input
	line  21:	unnamed         	input
	line  22:	unnamed         	input
	line  23:	unnamed         	input
	line  24:	unnamed         	input
	line  25:	unnamed         	input
	line  26:	unnamed         	input
	line  27:	unnamed         	input
	line  28:	unnamed         	input
	line  29:	unnamed         	input
	line  30:	unnamed         	input
	line  31:	unnamed         	input
	line  32:	"USB_RST"       	output active-low consumer="reset"
	line  33:	"GPIO27"        	input
	line  34:	unnamed         	input
	line  35:	unnamed         	input
	line  36:	"GPIO26"        	input
	line  37:	unnamed         	input
	line  38:	"GPIO4"         	input
	line  39:	unnamed         	input
	line  40:	unnamed         	input
	line  41:	"GPIO22"        	input
	line  42:	"GPIO25"        	input
	line  43:	unnamed         	input
	line  44:	unnamed         	input
	line  45:	unnamed         	input
	line  46:	unnamed         	input
	line  47:	unnamed         	input
	line  48:	unnamed         	input
	line  49:	unnamed         	input
	line  50:	unnamed         	input
	line  51:	unnamed         	input
	line  52:	unnamed         	input
	line  53:	unnamed         	input
	line  54:	unnamed         	input
	line  55:	unnamed         	input
	line  56:	unnamed         	input
	line  57:	unnamed         	input
	line  58:	unnamed         	input
	line  59:	unnamed         	input
	line  60:	unnamed         	input
	line  61:	unnamed         	input
	line  62:	unnamed         	input
	line  63:	unnamed         	input
	line  64:	unnamed         	input
	line  65:	unnamed         	input
	line  66:	unnamed         	input
	line  67:	unnamed         	input
	line  68:	unnamed         	input
	line  69:	unnamed         	input
	line  70:	unnamed         	input
	line  71:	unnamed         	input
	line  72:	unnamed         	input
	line  73:	unnamed         	input
	line  74:	unnamed         	input
	line  75:	unnamed         	input
	line  76:	unnamed         	input
	line  77:	unnamed         	input
	line  78:	unnamed         	input
	line  79:	unnamed         	input
	line  80:	unnamed         	input
	line  81:	unnamed         	input
	line  82:	unnamed         	input
	line  83:	unnamed         	input
	line  84:	unnamed         	input
	line  85:	unnamed         	input
	line  86:	unnamed         	input
gpiochip2 - 73 lines:
	line   0:	unnamed         	input
	line   1:	unnamed         	input
	line   2:	unnamed         	input
	line   3:	unnamed         	input
	line   4:	unnamed         	input
	line   5:	unnamed         	input
	line   6:	unnamed         	input
	line   7:	"GPIO16"        	input
	line   8:	"GPIO17"        	input
	line   9:	"GPIO21"        	input
	line  10:	"GPIO20"        	input
	line  11:	"GPIO18"        	input
	line  12:	"GPIO19"        	input
	line  13:	"GPIO15"        	input
	line  14:	"GPIO14"        	input
	line  15:	"GPIO5"         	input
	line  16:	"GPIO12"        	input
	line  17:	"GPIO6"         	input
	line  18:	"GPIO13"        	input
	line  19:	unnamed         	input
	line  20:	unnamed         	input
	line  21:	unnamed         	input
	line  22:	unnamed         	input
	line  23:	unnamed         	input
	line  24:	unnamed         	input
	line  25:	unnamed         	input
	line  26:	unnamed         	input
	line  27:	unnamed         	input
	line  28:	unnamed         	input
	line  29:	unnamed         	input
	line  30:	unnamed         	input
	line  31:	unnamed         	input
	line  32:	unnamed         	input
	line  33:	unnamed         	input
	line  34:	unnamed         	input
	line  35:	unnamed         	input
	line  36:	unnamed         	input
	line  37:	unnamed         	input
	line  38:	unnamed         	input
	line  39:	unnamed         	input
	line  40:	unnamed         	input
	line  41:	unnamed         	input
	line  42:	unnamed         	input
	line  43:	unnamed         	input
	line  44:	unnamed         	input
	line  45:	unnamed         	input
	line  46:	unnamed         	input
	line  47:	unnamed         	input
	line  48:	unnamed         	input active-low consumer=cd
	line  49:	unnamed         	output consumer=tlv71033
	line  50:	unnamed         	output consumer=regulator-3
	line  51:	unnamed         	input
	line  52:	unnamed         	input
	line  53:	unnamed         	input
	line  54:	unnamed         	input
	line  55:	unnamed         	input
	line  56:	unnamed         	input
	line  57:	unnamed         	input
	line  58:	unnamed         	input
	line  59:	unnamed         	input
	line  60:	unnamed         	input
	line  61:	unnamed         	input
	line  62:	unnamed         	input
	line  63:	unnamed         	input
	line  64:	unnamed         	input
	line  65:	unnamed         	input
	line  66:	unnamed         	input
	line  67:	unnamed         	input
	line  68:	unnamed         	input
	line  69:	unnamed         	input
	line  70:	unnamed         	input
	line  71:	unnamed         	input
	line  72:	unnamed         	input
melissa@beagle:~$ gpioinfo /dev/gpiochip2
gpioinfo: cannot find line '/dev/gpiochip2'
melissa@beagle:~$ gpioinfo gpiochip2
gpioinfo: cannot find line 'gpiochip2'
melissa@beagle:~$ gpioinfo 15
gpioinfo: cannot find line '15'
melissa@beagle:~$ gpioinfo 2 15
gpioinfo: cannot find line '2'
gpioinfo: cannot find line '15'
melissa@beagle:~$ gpioinfo gpiochip2 -c
gpioinfo: cannot find line 'gpiochip2'
gpioinfo: cannot find line '-c'
melissa@beagle:~$ gpioinfo -c gpiochip2 
gpiochip2 - 73 lines:
	line   0:	unnamed         	input
	line   1:	unnamed         	input
	line   2:	unnamed         	input
	line   3:	unnamed         	input
	line   4:	unnamed         	input
	line   5:	unnamed         	input
	line   6:	unnamed         	input
	line   7:	"GPIO16"        	input
	line   8:	"GPIO17"        	input
	line   9:	"GPIO21"        	input
	line  10:	"GPIO20"        	input
	line  11:	"GPIO18"        	input
	line  12:	"GPIO19"        	input
	line  13:	"GPIO15"        	input
	line  14:	"GPIO14"        	input
	line  15:	"GPIO5"         	input
	line  16:	"GPIO12"        	input
	line  17:	"GPIO6"         	input
	line  18:	"GPIO13"        	input
	line  19:	unnamed         	input
	line  20:	unnamed         	input
	line  21:	unnamed         	input
	line  22:	unnamed         	input
	line  23:	unnamed         	input
	line  24:	unnamed         	input
	line  25:	unnamed         	input
	line  26:	unnamed         	input
	line  27:	unnamed         	input
	line  28:	unnamed         	input
	line  29:	unnamed         	input
	line  30:	unnamed         	input
	line  31:	unnamed         	input
	line  32:	unnamed         	input
	line  33:	unnamed         	input
	line  34:	unnamed         	input
	line  35:	unnamed         	input
	line  36:	unnamed         	input
	line  37:	unnamed         	input
	line  38:	unnamed         	input
	line  39:	unnamed         	input
	line  40:	unnamed         	input
	line  41:	unnamed         	input
	line  42:	unnamed         	input
	line  43:	unnamed         	input
	line  44:	unnamed         	input
	line  45:	unnamed         	input
	line  46:	unnamed         	input
	line  47:	unnamed         	input
	line  48:	unnamed         	input active-low consumer=cd
	line  49:	unnamed         	output consumer=tlv71033
	line  50:	unnamed         	output consumer=regulator-3
	line  51:	unnamed         	input
	line  52:	unnamed         	input
	line  53:	unnamed         	input
	line  54:	unnamed         	input
	line  55:	unnamed         	input
	line  56:	unnamed         	input
	line  57:	unnamed         	input
	line  58:	unnamed         	input
	line  59:	unnamed         	input
	line  60:	unnamed         	input
	line  61:	unnamed         	input
	line  62:	unnamed         	input
	line  63:	unnamed         	input
	line  64:	unnamed         	input
	line  65:	unnamed         	input
	line  66:	unnamed         	input
	line  67:	unnamed         	input
	line  68:	unnamed         	input
	line  69:	unnamed         	input
	line  70:	unnamed         	input
	line  71:	unnamed         	input
	line  72:	unnamed         	input
melissa@beagle:~$ gpioinfo -c gpiochip2 15
gpiochip2 15	"GPIO5"         	input
melissa@beagle:~$ # read both A and B lines
sudo gpioget -c 2 5 6
"5"=inactive "6"=inactive
melissa@beagle:~$ sudo gpioget -c 2 GPIO5 GPIO6
"GPIO5"=active "GPIO6"=active
melissa@beagle:~$ sudo gpiomon -c 2 5 6
^C
melissa@beagle:~$ sudo gpiomon -c 2 GPIO5 GPIO6
1124.883605060	falling	gpiochip2 15 "GPIO5"
1124.904535150	falling	gpiochip2 17 "GPIO6"
1124.906398795	rising	gpiochip2 15 "GPIO5"
1124.908588765	rising	gpiochip2 17 "GPIO6"
1124.940765490	falling	gpiochip2 15 "GPIO5"
1124.950330605	falling	gpiochip2 17 "GPIO6"
1124.951803365	rising	gpiochip2 15 "GPIO5"
1124.953495115	rising	gpiochip2 17 "GPIO6"
1124.986272010	falling	gpiochip2 15 "GPIO5"
1124.991683160	falling	gpiochip2 17 "GPIO6"
1125.004602295	rising	gpiochip2 15 "GPIO5"
1125.006201010	rising	gpiochip2 17 "GPIO6"
1125.814590665	falling	gpiochip2 15 "GPIO5"
1125.817583775	falling	gpiochip2 17 "GPIO6"
1125.821519265	rising	gpiochip2 15 "GPIO5"
1125.822271880	rising	gpiochip2 17 "GPIO6"
1125.877964005	falling	gpiochip2 15 "GPIO5"
1125.884769820	falling	gpiochip2 17 "GPIO6"
1125.891566405	rising	gpiochip2 15 "GPIO5"
1125.893124785	rising	gpiochip2 17 "GPIO6"
1126.155367875	falling	gpiochip2 15 "GPIO5"
1126.164835070	falling	gpiochip2 17 "GPIO6"
1126.171852060	rising	gpiochip2 15 "GPIO5"
1126.173305495	rising	gpiochip2 17 "GPIO6"
1126.249944230	falling	gpiochip2 15 "GPIO5"
1126.265729235	falling	gpiochip2 17 "GPIO6"
1126.277261345	rising	gpiochip2 15 "GPIO5"
1126.279249955	rising	gpiochip2 17 "GPIO6"
1126.969242900	falling	gpiochip2 15 "GPIO5"
1126.974755190	falling	gpiochip2 17 "GPIO6"
1126.979435610	rising	gpiochip2 15 "GPIO5"
1126.980957735	rising	gpiochip2 17 "GPIO6"
1127.011098240	falling	gpiochip2 15 "GPIO5"
1127.018439850	falling	gpiochip2 17 "GPIO6"
1127.023318930	rising	gpiochip2 15 "GPIO5"
1127.025142505	rising	gpiochip2 17 "GPIO6"
1127.072271740	falling	gpiochip2 15 "GPIO5"
1127.083906675	falling	gpiochip2 17 "GPIO6"
1127.092015350	rising	gpiochip2 15 "GPIO5"
1127.094037795	rising	gpiochip2 17 "GPIO6"
1127.094043865	rising	gpiochip2 17 "GPIO6"
1127.094052270	rising	gpiochip2 17 "GPIO6"
1127.822910760	falling	gpiochip2 15 "GPIO5"
1127.829232995	falling	gpiochip2 17 "GPIO6"
1127.833003710	rising	gpiochip2 15 "GPIO5"
1127.834181705	rising	gpiochip2 17 "GPIO6"
1127.858750540	falling	gpiochip2 15 "GPIO5"
1127.866981955	falling	gpiochip2 17 "GPIO6"
1127.870714930	rising	gpiochip2 15 "GPIO5"
1127.871801135	rising	gpiochip2 17 "GPIO6"
1127.879973840	falling	gpiochip2 15 "GPIO5"
1127.884122250	falling	gpiochip2 17 "GPIO6"
1127.886375885	rising	gpiochip2 15 "GPIO5"
1127.887204760	rising	gpiochip2 17 "GPIO6"
1127.890001600	falling	gpiochip2 15 "GPIO5"
1127.892366830	falling	gpiochip2 17 "GPIO6"
1127.894205510	rising	gpiochip2 15 "GPIO5"
1127.894936510	rising	gpiochip2 17 "GPIO6"
1127.897103025	falling	gpiochip2 15 "GPIO5"
1127.899527885	falling	gpiochip2 17 "GPIO6"
1127.901210285	rising	gpiochip2 15 "GPIO5"
1127.901886345	rising	gpiochip2 17 "GPIO6"
1127.906119370	falling	gpiochip2 15 "GPIO5"
1127.910878285	falling	gpiochip2 17 "GPIO6"
1127.914430735	rising	gpiochip2 15 "GPIO5"
1127.915793615	rising	gpiochip2 17 "GPIO6"
1128.025841115	falling	gpiochip2 15 "GPIO5"
1128.037805515	falling	gpiochip2 17 "GPIO6"
1128.041942060	rising	gpiochip2 15 "GPIO5"
1128.044390710	rising	gpiochip2 17 "GPIO6"
1128.076192310	falling	gpiochip2 15 "GPIO5"
1128.082647990	falling	gpiochip2 17 "GPIO6"
1128.085425095	rising	gpiochip2 15 "GPIO5"
1128.087483795	rising	gpiochip2 17 "GPIO6"
1128.111185330	falling	gpiochip2 15 "GPIO5"
1128.115588035	falling	gpiochip2 17 "GPIO6"
1128.120260015	rising	gpiochip2 15 "GPIO5"
1128.123742180	rising	gpiochip2 17 "GPIO6"
1128.148990685	falling	gpiochip2 15 "GPIO5"
1128.161296165	falling	gpiochip2 17 "GPIO6"
1128.165205775	rising	gpiochip2 15 "GPIO5"
1128.168211960	rising	gpiochip2 17 "GPIO6"
1128.193581765	falling	gpiochip2 15 "GPIO5"
1128.220819850	falling	gpiochip2 17 "GPIO6"
1128.226165695	rising	gpiochip2 15 "GPIO5"
1128.229824380	rising	gpiochip2 17 "GPIO6"
1128.262445240	falling	gpiochip2 15 "GPIO5"
1128.289906425	falling	gpiochip2 17 "GPIO6"
1128.293975610	rising	gpiochip2 15 "GPIO5"
1128.297296300	rising	gpiochip2 17 "GPIO6"
1128.322920120	falling	gpiochip2 15 "GPIO5"
1128.323021215	rising	gpiochip2 15 "GPIO5"
1128.323250650	falling	gpiochip2 15 "GPIO5"
1128.380361195	falling	gpiochip2 17 "GPIO6"
1128.383414785	rising	gpiochip2 15 "GPIO5"
1128.386438575	rising	gpiochip2 17 "GPIO6"
1128.407339980	falling	gpiochip2 15 "GPIO5"
1128.442083885	falling	gpiochip2 17 "GPIO6"
1128.446141800	rising	gpiochip2 15 "GPIO5"
1128.451286475	rising	gpiochip2 17 "GPIO6"
1128.514805185	falling	gpiochip2 15 "GPIO5"
1128.547636380	falling	gpiochip2 17 "GPIO6"
1128.551573935	rising	gpiochip2 15 "GPIO5"
1128.556937945	rising	gpiochip2 17 "GPIO6"
1128.596647040	falling	gpiochip2 15 "GPIO5"
1128.612733975	falling	gpiochip2 17 "GPIO6"
1128.615799170	rising	gpiochip2 15 "GPIO5"
1128.620483910	rising	gpiochip2 17 "GPIO6"
1128.683179395	falling	gpiochip2 15 "GPIO5"
1128.686706840	falling	gpiochip2 17 "GPIO6"
1128.697862140	rising	gpiochip2 15 "GPIO5"
1128.699521020	rising	gpiochip2 17 "GPIO6"
1128.767772045	falling	gpiochip2 15 "GPIO5"
1128.775473850	falling	gpiochip2 17 "GPIO6"
1128.793030425	rising	gpiochip2 15 "GPIO5"
1128.795635380	rising	gpiochip2 17 "GPIO6"
1128.870710520	falling	gpiochip2 15 "GPIO5"
1128.880009200	falling	gpiochip2 17 "GPIO6"
1128.905846260	rising	gpiochip2 15 "GPIO5"
1128.910620490	rising	gpiochip2 17 "GPIO6"
1128.970102565	falling	gpiochip2 15 "GPIO5"
1128.977647870	falling	gpiochip2 17 "GPIO6"
1128.990087740	rising	gpiochip2 15 "GPIO5"
1128.992722800	rising	gpiochip2 17 "GPIO6"
1129.036766265	falling	gpiochip2 15 "GPIO5"
1129.043586360	falling	gpiochip2 17 "GPIO6"
1129.052189610	rising	gpiochip2 15 "GPIO5"
1129.054225970	rising	gpiochip2 17 "GPIO6"
1129.083176360	falling	gpiochip2 15 "GPIO5"
1129.092331205	falling	gpiochip2 17 "GPIO6"
1129.097924725	rising	gpiochip2 15 "GPIO5"
1129.100686125	rising	gpiochip2 17 "GPIO6"
1129.123044030	falling	gpiochip2 15 "GPIO5"
1129.131956900	falling	gpiochip2 17 "GPIO6"
1129.138417785	rising	gpiochip2 15 "GPIO5"
1129.141698620	rising	gpiochip2 17 "GPIO6"
1138.240265670	falling	gpiochip2 15 "GPIO5"
1138.257242390	falling	gpiochip2 17 "GPIO6"
1138.263882655	rising	gpiochip2 15 "GPIO5"
1138.265451530	rising	gpiochip2 17 "GPIO6"
1138.526657710	falling	gpiochip2 15 "GPIO5"
1138.540953895	falling	gpiochip2 17 "GPIO6"
1138.548040795	rising	gpiochip2 15 "GPIO5"
1138.550145635	rising	gpiochip2 17 "GPIO6"
1138.710805720	falling	gpiochip2 15 "GPIO5"
1138.726484645	falling	gpiochip2 17 "GPIO6"
1138.733709885	rising	gpiochip2 15 "GPIO5"
1138.735422015	rising	gpiochip2 17 "GPIO6"
1138.783019425	falling	gpiochip2 15 "GPIO5"
1138.793184820	falling	gpiochip2 17 "GPIO6"
1138.799149200	rising	gpiochip2 15 "GPIO5"
1138.800891810	rising	gpiochip2 17 "GPIO6"
1138.836484675	falling	gpiochip2 15 "GPIO5"
1138.842724430	falling	gpiochip2 17 "GPIO6"
1138.846260565	rising	gpiochip2 15 "GPIO5"
1138.847477490	rising	gpiochip2 17 "GPIO6"
1138.856474970	falling	gpiochip2 15 "GPIO5"
1138.861682820	falling	gpiochip2 17 "GPIO6"
1138.864400595	rising	gpiochip2 15 "GPIO5"
1138.865469290	rising	gpiochip2 17 "GPIO6"
1138.875360250	falling	gpiochip2 15 "GPIO5"
1138.880419400	falling	gpiochip2 17 "GPIO6"
1138.883123160	rising	gpiochip2 15 "GPIO5"
1138.884480930	rising	gpiochip2 17 "GPIO6"
1138.898990620	falling	gpiochip2 15 "GPIO5"
1138.906119055	falling	gpiochip2 17 "GPIO6"
1138.909939485	rising	gpiochip2 15 "GPIO5"
1138.911836100	rising	gpiochip2 17 "GPIO6"
1138.934262155	falling	gpiochip2 15 "GPIO5"
1138.939936005	falling	gpiochip2 17 "GPIO6"
1138.943104370	rising	gpiochip2 15 "GPIO5"
1138.945481090	rising	gpiochip2 17 "GPIO6"
1138.987116775	falling	gpiochip2 15 "GPIO5"
1138.995868710	falling	gpiochip2 17 "GPIO6"
1139.006950705	rising	gpiochip2 15 "GPIO5"
1139.014944560	rising	gpiochip2 17 "GPIO6"
^C
melissa@beagle:~$ 

