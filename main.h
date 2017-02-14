#ifndef DEFINE_H
#define DEFINE_H

#include <QApplication>

#define ADDR 6000
const quint16 GuestLogin        = 1000; //登录
const quint16 GuestLoginError   = 1001; //登录出错
const quint16 AdminLogin        = 1006; //登录
const quint16 AdminLoginError   = 1007; //登录出错
const quint16 AdminLoginSuccess = 1008; //登录成功

const quint16 GuestDisplay      = 1004; //显示信息

const quint16 GuestDroped       = 1005; //下线

const quint16 GuestTransmit     = 1101; //消息转发
const quint16 BuildTransmit     = 1102; //建立转发
const quint16 BreakTransmit     = 1103; //取消转发

const quint16 ServerGetHead     = 1002; //服务器获取文件
const quint16 ClientGetHead     = 1003; //客户端获取文件
const quint16 FileHead          = 2000; //文件头
const quint16 FileHeadError     = 2009; //文件头错误
const quint16 FileData          = 2001; //文件内容
const quint16 FileDataError     = 2003; //文件内容错误
const quint16 FileDataSuccess   = 2002; //文件发送成功

const quint16 GuestCommand      = 2004; //发送系统命令
const quint16 GuestProcess      = 2005; //系统命令返回
const quint16 GuestDevices      = 2006; //获取在线设备列表

const quint16 GuestHeart        = 6000;

#define TCP_CMD_CLIENT_LOGIN        1000 //登录
#define TCP_CMD_CLIENT_LOGIN_ERROR  1001 //登录出错
#define TCP_CMD_SERVER_GET_HEAD     1002 //服务器获取文件信息
#define TCP_CMD_CLIENT_GET_HEAD     1003 //客户端获取文件信息
#define TCP_CMD_SOCKET_DISPLAY      1004
#define TCP_CMD_CLIENT_DROPPED      1005 //客户端下线
#define TCP_CMD_LOCAL_LOGIN         1006 //客户端登录
#define TCP_CMD_LOGIN_ERROR         1007 //
#define TCP_CMD_LOGIN_SUCCESS       1008
#define TCP_CMD_HEAD_DATA           2000
#define TCP_CMD_HEAD_ERROR          2009
#define TCP_CMD_FILE_DATA           2001
#define TCP_CMD_FILE_ERROR          2003
#define TCP_CMD_FILE_SUCCESS        2002
#define TCP_CMD_SHEEL_CMD           2004
#define TCP_CMD_SHEEL_RETURN        2005
#define TCP_CMD_LIST_GET            2006
#define TCP_CMD_LIST_ERROR          2007
#define TCP_CMD_FILE_LIST           2008
#define TCP_CMD_HEART               6000

#define ADDR 6000
#define WIN_CMD_SWITCH  7000
#define WIN_CMD_SHOW    7001
#define WIN_CMD_RESULT  7002
#define WIN_CMD_JUDGE   7003
#define WIN_CMD_TEMP    7004
#define WIN_WAVE_BYTE   7005
#define WIN_WAVE_TEST   7006
#define WIN_WAVE_ITEM   7007
#define WIN_CMD_DEBUG   7008
#define WIN_CMD_WAVE    7009

#define CAN_DAT_GET 8000
#define CAN_DAT_PUT 8001

#define CTRL_CMD_STATE 9000
#define CTRL_CMD_START 9001
#define CTRL_CMD_STOP  9002
#define CTRL_CMD_CONFIG 9003

#define  ResCu    0.0039  //  铜材料
#define  ResCu_Al 0.0041  //  铜铝材料
#define  ResAl    0.0043  //  铝材料


#define NET "./network/"
#define TMP "./temp/"
#define CON "./config/"

#ifdef __arm__
#define GLOBAL_SET "/mnt/nandflash/AIP/Sys.ini"
#else
#define GLOBAL_SET "settings/global.ini"
#endif

#define LOCAL

#define CAN_ID_DLR       0x41         // 电阻板
#define CAN_ID_DLR_WAVE  0x441        // 电阻板反嵌波形
#define CAN_ID_IR        0x61         // 耐压板
#define CAN_ID_IMP       0x81         // 匝间板
#define CAN_ID_IMP_WAVE  0x481        // 匝间板波形
#define CAN_ID_INDL      0xC1
#define CAN_ID_13OUT     0x261        // 输出板13
#define CAN_ID_14OUT     0x281        // 输出板14
#define CAN_ID_15OUT     0x2A1        // 输出板15
#define CAN_ID_16OUT     0x2C1        // 输出板16
#define CAN_ID_17OUT     0x2E1        // 输出板17

#define CAN_CMD_STATE        0x00     //上传状态
#define CAN_CMD_RESULT_ONE   0x01     //上传结果1
#define CAN_CMD_RESULT_TWO   0x02     //上传结果2
#define CAN_CMD_WAVE         0x03     //上传波形
#define CAN_CMD_DEBUG        0x06     //上传调试参数
#define CAN_CMD_VERSION      0x08     //上传版本

#define WIN_ID_DLR      0x01
#define WIN_ID_MAG      0x02
#define WIN_ID_IR       0x03
#define WIN_ID_ACW      0x04
#define WIN_ID_DCW      0x05
#define WIN_ID_IMP      0x06
#define WIN_ID_INDL     0x07

//extern

#endif // DEFINE_H
