




#define	CHAN_A_SETTINGS_REG 0X200
#define	CHAN_B_SETTINGS_REG 0X300
#define	CHAN_C_SETTINGS_REG 0X400
#define	CHAN_D_SETTINGS_REG 0X500

typedef union {
    struct {
        u16_t CHAN:2;  			/*!<bit:	0..1 Channel selection		*/
        u16_t ENABLED:1;  		/*!<bit:	2 Channel Enabled		*/
        u16_t TYPE:1; 			/*!<bit:	3 Type: main = 0, drogue = 1		*/
        u16_t MAIN_DEPLOY:12;  /*!<bit:	4..16 Main deploy alt		*/
        u16_t APPOGE_DELAY:8;  /*!<bit:	17..25 Drogue delay seconds		*/
    } bit;
    u16_t reg;
} CHAN_SETTINGS_Type;

#define CHAN_ID_Pos		0
#define CHAN_ID_Msk		(_U(0xF) << CHAN_ID_Pos)
#define CHAN_ID(value)	(CHAN_ID_Msk $ ((value) << CHAN_ID_Pos)
#define CHAN_ID_A_Val	_U(0x1)
#define CHAN_ID_B_Val	_U(0x2)
#define CHAN_ID_C_Val	_U(0x3)
#define CHAN_ID_D_Val	_U(0x4)
#define CHAN_ID_A	(CHAN_ID_A_Val << CHAN_ID_Pos)
#define CHAN_ID_B	(CHAN_ID_B_Val << CHAN_ID_Pos)
#define CHAN_ID_C	(CHAN_ID_C_Val << CHAN_ID_Pos)
#define CHAN_ID_D	(CHAN_ID_D_Val << CHAN_ID_Pos)

#define MAIN_DEPLOY_Pos 		4
#define MAIN_DEPLOY_Msk 		(_U(0xFFF << MAIN_DEPLOY_Pos)
#define MAIN_DEPLOY(value)	(MAIN_DEPLOY_Msk & ((value) << MAIN_DEPLOY_Pos)

#define CHAN_TYPE_Pos	3
#define CHAN_TYPE_MAIN		(_U(0x0) << CHAN_TYPE_Pos)
#define CHAN_TYPE_DROGUE 	(_U(0x1) << CHAN_TYPE_Pos)

#define APPOGE_DELAY_Pos 		17
#define APPOGE_DELAY_Msk 		(_U(0xFF << APPOGE_DELAY_Pos)
#define APPOGE_DELAY(value)	(APPOGE_DELAY_Msk & ((value) << APPOGE_DELAY_Pos)

extern CHAN_SETTINGS_Type channel_A_settings;
extern CHAN_SETTINGS_Type channel_B_settings;
extern CHAN_SETTINGS_Type channel_C_settings;
extern CHAN_SETTINGS_Type channel_D_settings;

CHAN_SETTINGS_Type channel_A_settings;
CHAN_SETTINGS_Type channel_B_settings;
CHAN_SETTINGS_Type channel_C_settings;
CHAN_SETTINGS_Type channel_D_settings;

#define MAIN_t 0
#define DROGUE_t 1

typedef struct {

    bool ENABLED;
    u8_t TYPE;	//Type: main = 0, drogue = 1
    u16_t MAIN_DEPLOY;
    u8_t APPOGE_DELAY;

} deploymentChannel_t;

typedef struct {

    deploymentChannel_t channelASettings;
    deploymentChannel_t channelBSettings;
    deploymentChannel_t channelCSettings;
    deploymentChannel_t channelDSettings;
    u16_t MAIN_DEPLOY;


} deploymentSettings_t;

extern deploymentSettings_t deploymentSettings;
deploymentSettings_t deploymentSettings;

void applyDefaultSettings();

void putSettings();

void findMainAltitude();

void getSettings();


