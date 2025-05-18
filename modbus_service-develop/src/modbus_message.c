
#include "can_message.h"

can_msg_t can_msg_tbl[CAN_MSG_MAX] = {
	// CAN_MSG_REC_SET_WALK_IN
	{
		.frame = {
			.can_id  = 0x82933ff0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 2,
		.sig_tbl = {
			CAN_SIG_REC_WALK_IN_TIME,
			CAN_SIG_REC_WALK_IN_ENABLE,
		}
	},
	// CAN_MSG_FAN_JIG_RESPOND
	{
		.frame = {
			.can_id  = 0xa,
			.can_dlc = 4,
			.data    = { 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 4,
		.sig_tbl = {
			CAN_SIG_FAN_JIG_RES_DATA_VALUE2,
			CAN_SIG_FAN_JIG_RES_DATA_VALUE1,
			CAN_SIG_FAN_JIG_RES_DATA_LENGTH,
			CAN_SIG_FAN_JIG_RES_TARGET_ADDRESS,
		}
	},
	// CAN_MSG_FAN_JIG_REQUEST
	{
		.frame = {
			.can_id  = 0x0,
			.can_dlc = 3,
			.data    = { 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 3,
		.sig_tbl = {
			CAN_SIG_FAN_JIG_REQ_DATA_VALUE1,
			CAN_SIG_FAN_JIG_REQ_DATA_LENGTH,
			CAN_SIG_FAN_JIG_REQ_TARGET_ADDRESS,
		}
	},
	// CAN_MSG_REC1_GROUP_ACK
	{
		.frame = {
			.can_id  = 0x8296f001,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 1,
		.sig_tbl = {
			CAN_SIG_REC1_GROUP_CONFIRMED,
		}
	},
	// CAN_MSG_REC0_GROUP_ACK
	{
		.frame = {
			.can_id  = 0x8296f000,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 1,
		.sig_tbl = {
			CAN_SIG_REC0_GROUP_CONFIRMED,
		}
	},
	// CAN_MSG_REC1_GROUP_SET_MSG
	{
		.frame = {
			.can_id  = 0x829601f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 1000,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 1,
		.sig_tbl = {
			CAN_SIG_REC1_GROUP_NUMBER,
		}
	},
	// CAN_MSG_REC0_GROUP_SET_MSG
	{
		.frame = {
			.can_id  = 0x829600f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 1000,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 1,
		.sig_tbl = {
			CAN_SIG_REC0_GROUP_NUMBER,
		}
	},
	// CAN_MSG_REC_ADDRESSING_MODE_MSG
	{
		.frame = {
			.can_id  = 0x829f3ff0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 1000,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 1,
		.sig_tbl = {
			CAN_SIG_REC_ADDRESSING_MODE,
		}
	},
	// CAN_MSG_CP1_MONITOR
	{
		.frame = {
			.can_id  = 0x80172300,
			.can_dlc = 2,
			.data    = { 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 2,
		.sig_tbl = {
			CAN_SIG_MONITOR_PE1_LOST,
			CAN_SIG_MONITOR_CP1_LOST,
		}
	},
	// CAN_MSG_CP1_INFOVERSION
	{
		.frame = {
			.can_id  = 0x80172200,
			.can_dlc = 2,
			.data    = { 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 3,
		.sig_tbl = {
			CAN_SIG_CP1_APP_SUBMINOR,
			CAN_SIG_CP1_APP_MINOR,
			CAN_SIG_CP1_APP_MAJOR,
		}
	},
	// CAN_MSG_CP0_INFOVERSION
	{
		.frame = {
			.can_id  = 0x80172100,
			.can_dlc = 2,
			.data    = { 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 3,
		.sig_tbl = {
			CAN_SIG_CP0_APP_SUBMINOR,
			CAN_SIG_CP0_APP_MINOR,
			CAN_SIG_CP0_APP_MAJOR,
		}
	},
	// CAN_MSG_IO_EXT_INFOVERSION
	{
		.frame = {
			.can_id  = 0x80171400,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 4,
		.sig_tbl = {
			CAN_SIG_AC_RELAY_STATUS,
			CAN_SIG_IO_VERSION_APP_MINOR,
			CAN_SIG_IO_VERSION_APP_MAJOR,
			CAN_SIG_IO_VERSION_APP_SUBMINOR,
		}
	},
	// CAN_MSG_REC0_ON_OFF_REQUEST
	{
		.frame = {
			.can_id  = 0x82da01f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 1000,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 1,
		.sig_tbl = {
			CAN_SIG_REC0_ON_OFF,
		}
	},
	// CAN_MSG_REC0_OUTPUT_REQUEST
	{
		.frame = {
			.can_id  = 0x82dc01f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 500,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 2,
		.sig_tbl = {
			CAN_SIG_REC0_VOLTAGE_REQUEST,
			CAN_SIG_REC0_CURRENT_REQUEST,
		}
	},
	// CAN_MSG_REC0_OUTPUT_INFO_REQUEST
	{
		.frame = {
			.can_id  = 0x82c101f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 500,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 0,
	},
	// CAN_MSG_REC0_OUTPUT_INFO_RESPONSE
	{
		.frame = {
			.can_id  = 0x82c1f001,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 2,
		.sig_tbl = {
			CAN_SIG_REC0_CURRENT_PRESENT,
			CAN_SIG_REC0_VOLTAGE_PRESENT,
		}
	},
	// CAN_MSG_REC0_ERROR_INFO_REQUEST
	{
		.frame = {
			.can_id  = 0x828400f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 500,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 0,
	},
	// CAN_MSG_REC0_ERROR_INFO_RESPONSE
	{
		.frame = {
			.can_id  = 0x8284f000,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 22,
		.sig_tbl = {
			CAN_SIG_REC0_TEMPERATURE,
			CAN_SIG_REC0_GROUP,
			CAN_SIG_REC0_STATE2_MDL_ID_REPETITION,
			CAN_SIG_REC0_STATE2_MDL_PFC_SIDE_OFF,
			CAN_SIG_REC0_STATE2_INPUT_OVER_VOLTAGE,
			CAN_SIG_REC0_STATE2_INPUT_UNDER_VOLTAGE,
			CAN_SIG_REC0_STATE2_INPUT_UNBALANCE,
			CAN_SIG_REC0_STATE2_INPUT_PHASE_LOST,
			CAN_SIG_REC0_STATE2_LOAD_UNSHARING,
			CAN_SIG_REC0_STATE2_POWER_LIMIT,
			CAN_SIG_REC0_STATE1_COMM_INTERRUPT,
			CAN_SIG_REC0_STATE1_WALK_IN_ENABLE,
			CAN_SIG_REC0_STATE1_OUTPUT_OVER_VOLTAGE,
			CAN_SIG_REC0_STATE1_OVER_TEMPERATURE,
			CAN_SIG_REC0_STATE1_FAN_FAULT,
			CAN_SIG_REC0_STATE1_MDL_PROTECTED,
			CAN_SIG_REC0_STATE1_MDL_FAULT,
			CAN_SIG_REC0_STATE1_MDL_OFF_STATE,
			CAN_SIG_REC0_STATE0_DISCHARGE_ABNORMAL,
			CAN_SIG_REC0_STATE0_PFC_SIDE_ABNORMAL,
			CAN_SIG_REC0_STATE0_INNER_COMM,
			CAN_SIG_REC0_STATE0_OUTPUT_SHORT,
		}
	},
	// CAN_MSG_REC1_ON_OFF_REQUEST
	{
		.frame = {
			.can_id  = 0x82da02f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 1000,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 1,
		.sig_tbl = {
			CAN_SIG_REC1_ON_OFF,
		}
	},
	// CAN_MSG_REC1_OUTPUT_REQUEST
	{
		.frame = {
			.can_id  = 0x82dc02f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 500,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 2,
		.sig_tbl = {
			CAN_SIG_REC1_VOLTAGE_REQUEST,
			CAN_SIG_REC1_CURRENT_REQUEST,
		}
	},
	// CAN_MSG_REC1_OUTPUT_INFO_REQUEST
	{
		.frame = {
			.can_id  = 0x82c102f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 500,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 0,
	},
	// CAN_MSG_REC1_OUTPUT_INFO_RESPONSE
	{
		.frame = {
			.can_id  = 0x82c1f002,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 2,
		.sig_tbl = {
			CAN_SIG_REC1_CURRENT_PRESENT,
			CAN_SIG_REC1_VOLTAGE_PRESENT,
		}
	},
	// CAN_MSG_REC1_ERROR_INFO_REQUEST
	{
		.frame = {
			.can_id  = 0x828401f0,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_TX_MSG,
		.period_ms  = 500,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 0,
	},
	// CAN_MSG_REC1_ERROR_INFO_RESPONSE
	{
		.frame = {
			.can_id  = 0x8284f001,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 0,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 22,
		.sig_tbl = {
			CAN_SIG_REC1_TEMPERATURE,
			CAN_SIG_REC1_GROUP,
			CAN_SIG_REC1_STATE2_MDL_ID_REPETITION,
			CAN_SIG_REC1_STATE2_MDL_PFC_SIDE_OFF,
			CAN_SIG_REC1_STATE2_INPUT_OVER_VOLTAGE,
			CAN_SIG_REC1_STATE2_INPUT_UNDER_VOLTAGE,
			CAN_SIG_REC1_STATE2_INPUT_UNBALANCE,
			CAN_SIG_REC1_STATE2_INPUT_PHASE_LOST,
			CAN_SIG_REC1_STATE2_LOAD_UNSHARING,
			CAN_SIG_REC1_STATE2_POWER_LIMIT,
			CAN_SIG_REC1_STATE1_COMM_INTERRUPT,
			CAN_SIG_REC1_STATE1_WALK_IN_ENABLE,
			CAN_SIG_REC1_STATE1_OUTPUT_OVER_VOLTAGE,
			CAN_SIG_REC1_STATE1_OVER_TEMPERATURE,
			CAN_SIG_REC1_STATE1_FAN_FAULT,
			CAN_SIG_REC1_STATE1_MDL_PROTECTED,
			CAN_SIG_REC1_STATE1_MDL_FAULT,
			CAN_SIG_REC1_STATE1_MDL_OFF_STATE,
			CAN_SIG_REC1_STATE0_DISCHARGE_ABNORMAL,
			CAN_SIG_REC1_STATE0_PFC_SIDE_ABNORMAL,
			CAN_SIG_REC1_STATE0_INNER_COMM,
			CAN_SIG_REC1_STATE0_OUTPUT_SHORT,
		}
	},
	// CAN_MSG_FAN_FAILDEM
	{
		.frame = {
			.can_id  = 0x80178100,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 8,
		.sig_tbl = {
			CAN_SIG_FAN7_FAILDEM,
			CAN_SIG_FAN6_FAILDEM,
			CAN_SIG_FAN5_FAILDEM,
			CAN_SIG_FAN4_FAILDEM,
			CAN_SIG_FAN3_FAILDEM,
			CAN_SIG_FAN2_FAILDEM,
			CAN_SIG_FAN1_FAILDEM,
			CAN_SIG_FAN0_FAILDEM,
		}
	},
	// CAN_MSG_FAN_TEMP_FAILDEM
	{
		.frame = {
			.can_id  = 0x80178200,
			.can_dlc = 3,
			.data    = { 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 3,
		.sig_tbl = {
			CAN_SIG_TEMP2_FAILDEM,
			CAN_SIG_TEMP1_FAILDEM,
			CAN_SIG_TEMP0_FAILDEM,
		}
	},
	// CAN_MSG_FAN_INFO_GROUP0
	{
		.frame = {
			.can_id  = 0x80178300,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 8,
		.sig_tbl = {
			CAN_SIG_FAN3_SPEED,
			CAN_SIG_FAN3_DUTY,
			CAN_SIG_FAN2_SPEED,
			CAN_SIG_FAN2_DUTY,
			CAN_SIG_FAN1_SPEED,
			CAN_SIG_FAN1_DUTY,
			CAN_SIG_FAN0_SPEED,
			CAN_SIG_FAN0_DUTY,
		}
	},
	// CAN_MSG_FAN_INFO_GROUP1
	{
		.frame = {
			.can_id  = 0x80178400,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 8,
		.sig_tbl = {
			CAN_SIG_FAN7_SPEED,
			CAN_SIG_FAN7_DUTY,
			CAN_SIG_FAN6_SPEED,
			CAN_SIG_FAN6_DUTY,
			CAN_SIG_FAN5_SPEED,
			CAN_SIG_FAN5_DUTY,
			CAN_SIG_FAN4_SPEED,
			CAN_SIG_FAN4_DUTY,
		}
	},
	// CAN_MSG_FAN_TEMP_INFO
	{
		.frame = {
			.can_id  = 0x80178500,
			.can_dlc = 3,
			.data    = { 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 3,
		.sig_tbl = {
			CAN_SIG_TEMP2_INFO,
			CAN_SIG_TEMP1_INFO,
			CAN_SIG_TEMP0_INFO,
		}
	},
	// CAN_MSG_FAN_VERSION
	{
		.frame = {
			.can_id  = 0x80178000,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 9,
		.sig_tbl = {
			CAN_SIG_FAN_HW_VERSION_SUBMINOR,
			CAN_SIG_FAN_HW_VERSION_MAJOR,
			CAN_SIG_FAN_VESION_BOOT_SUBMINOR,
			CAN_SIG_FAN_VERSION_BOOT_MINOR,
			CAN_SIG_FAN_VERSION_BOOT_MAJOR,
			CAN_SIG_FAN_VESION_APP_SUBMINOR,
			CAN_SIG_FAN_VERSION_APP_MINOR,
			CAN_SIG_FAN_VESION_APP_MAJOR,
			CAN_SIG_FAN_HW_VERSION_MINOR,
		}
	},
	// CAN_MSG_FAN_SYSTEM_FAILDEM
	{
		.frame = {
			.can_id  = 0x80178800,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 8,
		.sig_tbl = {
			CAN_SIG_FAN_SYSTEM2_FAILDEM_ALL_TEMPS,
			CAN_SIG_FAN_SYSTEM1_FAILDEM_ALL_TEMPS,
			CAN_SIG_FAN_SYSTEM2_COOLING_DONE,
			CAN_SIG_FAN_SYSTEM1_COOLING_DONE,
			CAN_SIG_FAN_SYSTEM2_OVER_TEMP,
			CAN_SIG_FAN_SYSTEM2_FAILDEM_ALL_FANS,
			CAN_SIG_FAN_SYSTEM1_OVER_TEMP,
			CAN_SIG_FAN_SYSTEM1_FAILDEM_ALL_FANS,
		}
	},
	// CAN_MSG_REC0_OUTPUT_NOTIFY
	{
		.frame = {
			.can_id  = 0x80178600,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_TX_MSG,
		.period_ms  = 1000,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 2,
		.sig_tbl = {
			CAN_SIG_REC0_CURRENT_NOTIFY,
			CAN_SIG_REC0_VOLTAGE_NOTIFY,
		}
	},
	// CAN_MSG_REC1_OUTPUT_NOTIFY
	{
		.frame = {
			.can_id  = 0x80178700,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_TX_MSG,
		.period_ms  = 1000,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 2,
		.sig_tbl = {
			CAN_SIG_REC1_VOLTAGE_NOTIFY,
			CAN_SIG_REC1_CURRENT_NOTIFY,
		}
	},
	// CAN_MSG_SYSTEM_MODE
	{
		.frame = {
			.can_id  = 0x80171500,
			.can_dlc = 4,
			.data    = { 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_TX_MSG,
		.period_ms  = 500,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 6,
		.sig_tbl = {
			CAN_SIG_AC_RELAY_REQUEST,
			CAN_SIG_SYSTEM_STATE,
			CAN_SIG_V2G1_STATE,
			CAN_SIG_V2G0_STATE,
			CAN_SIG_FAST_CHARGE,
			CAN_SIG_ACTIVE_OUTLET,
		}
	},
	// CAN_MSG_SYSTEM_OUTPUT_REQUEST
	{
		.frame = {
			.can_id  = 0x80171600,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_TX_MSG,
		.period_ms  = 500,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 4,
		.sig_tbl = {
			CAN_SIG_DC1_VOLTAGE_MAX,
			CAN_SIG_DC0_VOLTAGE_MAX,
			CAN_SIG_DC1_VOLTAGE_REQUEST,
			CAN_SIG_DC0_VOLTAGE_REQUEST,
		}
	},
	// CAN_MSG_HV_ALERT_STATUS
	{
		.frame = {
			.can_id  = 0x80176000,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 27,
		.sig_tbl = {
			CAN_SIG_HV_EXTMEM_FAIL,
			CAN_SIG_HV_SELFTEST_STATE,
			CAN_SIG_HV_AC_RELAY_STATUS,
			CAN_SIG_HV_OUTPUT_LV_RELAY_ERROR,
			CAN_SIG_HV_OUTPUT_HV_RELAY_ERROR,
			CAN_SIG_HV_OUTPUT_PRE_RELAY_ERROR,
			CAN_SIG_HV_OUTPUT1_LV_RELAY_ERROR,
			CAN_SIG_HV_OUTPUT1_HV_RELAY_ERROR,
			CAN_SIG_HV_OUTPUT1_PRE_RELAY_ERROR,
			CAN_SIG_HV_OUTPUT0_LV_RELAY_ERROR,
			CAN_SIG_HV_OUTPUT0_HV_RELAY_ERROR,
			CAN_SIG_HV_OUTPUT0_PRE_RELAY_ERROR,
			CAN_SIG_HV_OVERHEAT,
			CAN_SIG_HV_ISO_N1_ERROR,
			CAN_SIG_HV_ISO_P1_ERROR,
			CAN_SIG_HV_ISO_N0_ERROR,
			CAN_SIG_HV_ISO_P0_ERROR,
			CAN_SIG_HV_OUTPUT1_ABNORMAL,
			CAN_SIG_HV_OUTPUT1_UNDER_VOLTAGE,
			CAN_SIG_HV_OUTPUT1_OVER_VOLTAGE,
			CAN_SIG_HV_OUTPUT0_ABNORMAL,
			CAN_SIG_HV_OUTPUT0_UNDER_VOLTAGE,
			CAN_SIG_HV_OUTPUT0_OVER_VOLTAGE,
			CAN_SIG_HV_INPUT_UNDER_VOLTAGE,
			CAN_SIG_HV_INPUT_OVER_VOLTAGE,
			CAN_SIG_HV_STATUS1,
			CAN_SIG_HV_STATUS0,
		}
	},
	// CAN_MSG_HV_MONITOR
	{
		.frame = {
			.can_id  = 0x80176200,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 9,
		.sig_tbl = {
			CAN_SIG_HV_CHARGER_MODE,
			CAN_SIG_DC1_PRECHARGE_RELAY,
			CAN_SIG_DC1_MAIN_RELAY,
			CAN_SIG_DC1_ISOLATION_STATUS,
			CAN_SIG_DC1_VOLTAGE_PRESENT,
			CAN_SIG_DC0_PRECHARGE_RELAY,
			CAN_SIG_DC0_MAIN_RELAY,
			CAN_SIG_DC0_ISOLATION_STATUS,
			CAN_SIG_DC0_VOLTAGE_PRESENT,
		}
	},
	// CAN_MSG_HV_VERSION
	{
		.frame = {
			.can_id  = 0x80176100,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 6,
		.sig_tbl = {
			CAN_SIG_HV_VERSION_BOOT_SUBMINOR,
			CAN_SIG_HV_VERSION_BOOT_MINOR,
			CAN_SIG_HV_VERSION_BOOT_MAJOR,
			CAN_SIG_HV_VERSION_APP_SUBMINOR,
			CAN_SIG_HV_VERSION_APP_MINOR,
			CAN_SIG_HV_VERSION_APP_MAJOR,
		}
	},
	// CAN_MSG_HEART_BEAT
	{
		.frame = {
			.can_id  = 0x80171300,
			.can_dlc = 8,
			.data    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_TX_MSG,
		.period_ms  = 500,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 8,
		.sig_tbl = {
			CAN_SIG_SYSTEM_REBOOT,
			CAN_SIG_CONFIGURE_CHARGER_MODE,
			CAN_SIG_COMM_REQUEST_XMC1,
			CAN_SIG_COMM_REQUEST_XMC0,
			CAN_SIG_COMM_REQUEST_ROLL_COUNTER,
			CAN_SIG_COMM_REQUEST_EXT,
			CAN_SIG_COMM_REQUEST_HV,
			CAN_SIG_COMM_REQUEST_FAN,
		}
	},
	// CAN_MSG_CP0_MONITOR
	{
		.frame = {
			.can_id  = 0x80172000,
			.can_dlc = 2,
			.data    = { 0x00, 0x00 }
		},
		.bus_id     = 1,
		.type       = CAN_RX_MSG,
		.period_ms  = 0,
		.elapsed    = 0,
		.active     = false,
		.sig_cnt    = 2,
		.sig_tbl = {
			CAN_SIG_MONITOR_PE0_LOST,
			CAN_SIG_MONITOR_CP0_LOST,
		}
	},
};

const can_sig_t can_sig_tbl[CAN_SIG_MAX] = {
	{ .start = 48, .len = 16, .msg_id = CAN_MSG_REC_SET_WALK_IN           },		// CAN_SIG_REC_WALK_IN_TIME
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_REC_SET_WALK_IN           },		// CAN_SIG_REC_WALK_IN_ENABLE
	{ .start = 24, .len =  8, .msg_id = CAN_MSG_FAN_JIG_RESPOND           },		// CAN_SIG_FAN_JIG_RES_DATA_VALUE2
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_FAN_JIG_RESPOND           },		// CAN_SIG_FAN_JIG_RES_DATA_VALUE1
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_FAN_JIG_RESPOND           },		// CAN_SIG_FAN_JIG_RES_DATA_LENGTH
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_FAN_JIG_RESPOND           },		// CAN_SIG_FAN_JIG_RES_TARGET_ADDRESS
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_FAN_JIG_REQUEST           },		// CAN_SIG_FAN_JIG_REQ_DATA_VALUE1
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_FAN_JIG_REQUEST           },		// CAN_SIG_FAN_JIG_REQ_DATA_LENGTH
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_FAN_JIG_REQUEST           },		// CAN_SIG_FAN_JIG_REQ_TARGET_ADDRESS
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_REC1_GROUP_ACK            },		// CAN_SIG_REC1_GROUP_CONFIRMED
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_REC0_GROUP_ACK            },		// CAN_SIG_REC0_GROUP_CONFIRMED
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_REC1_GROUP_SET_MSG        },		// CAN_SIG_REC1_GROUP_NUMBER
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_REC0_GROUP_SET_MSG        },		// CAN_SIG_REC0_GROUP_NUMBER
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_REC_ADDRESSING_MODE_MSG   },		// CAN_SIG_REC_ADDRESSING_MODE
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_CP1_MONITOR               },		// CAN_SIG_MONITOR_PE1_LOST
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_CP1_MONITOR               },		// CAN_SIG_MONITOR_CP1_LOST
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_CP1_INFOVERSION           },		// CAN_SIG_CP1_APP_SUBMINOR
	{ .start =  4, .len =  4, .msg_id = CAN_MSG_CP1_INFOVERSION           },		// CAN_SIG_CP1_APP_MINOR
	{ .start =  0, .len =  4, .msg_id = CAN_MSG_CP1_INFOVERSION           },		// CAN_SIG_CP1_APP_MAJOR
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_CP0_INFOVERSION           },		// CAN_SIG_CP0_APP_SUBMINOR
	{ .start =  4, .len =  4, .msg_id = CAN_MSG_CP0_INFOVERSION           },		// CAN_SIG_CP0_APP_MINOR
	{ .start =  0, .len =  4, .msg_id = CAN_MSG_CP0_INFOVERSION           },		// CAN_SIG_CP0_APP_MAJOR
	{ .start = 56, .len =  4, .msg_id = CAN_MSG_IO_EXT_INFOVERSION        },		// CAN_SIG_AC_RELAY_STATUS
	{ .start =  4, .len =  4, .msg_id = CAN_MSG_IO_EXT_INFOVERSION        },		// CAN_SIG_IO_VERSION_APP_MINOR
	{ .start =  0, .len =  4, .msg_id = CAN_MSG_IO_EXT_INFOVERSION        },		// CAN_SIG_IO_VERSION_APP_MAJOR
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_IO_EXT_INFOVERSION        },		// CAN_SIG_IO_VERSION_APP_SUBMINOR
	{ .start =  0, .len = 64, .msg_id = CAN_MSG_REC0_ON_OFF_REQUEST       },		// CAN_SIG_REC0_ON_OFF
	{ .start =  0, .len = 32, .msg_id = CAN_MSG_REC0_OUTPUT_REQUEST       },		// CAN_SIG_REC0_VOLTAGE_REQUEST
	{ .start = 32, .len = 32, .msg_id = CAN_MSG_REC0_OUTPUT_REQUEST       },		// CAN_SIG_REC0_CURRENT_REQUEST
	{ .start = 32, .len = 32, .msg_id = CAN_MSG_REC0_OUTPUT_INFO_RESPONSE },		// CAN_SIG_REC0_CURRENT_PRESENT
	{ .start =  0, .len = 32, .msg_id = CAN_MSG_REC0_OUTPUT_INFO_RESPONSE },		// CAN_SIG_REC0_VOLTAGE_PRESENT
	{ .start = 32, .len =  8, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_TEMPERATURE
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_GROUP
	{ .start = 41, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE2_MDL_ID_REPETITION
	{ .start = 47, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE2_MDL_PFC_SIDE_OFF
	{ .start = 46, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE2_INPUT_OVER_VOLTAGE
	{ .start = 45, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE2_INPUT_UNDER_VOLTAGE
	{ .start = 44, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE2_INPUT_UNBALANCE
	{ .start = 43, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE2_INPUT_PHASE_LOST
	{ .start = 42, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE2_LOAD_UNSHARING
	{ .start = 40, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE2_POWER_LIMIT
	{ .start = 55, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE1_COMM_INTERRUPT
	{ .start = 54, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE1_WALK_IN_ENABLE
	{ .start = 53, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE1_OUTPUT_OVER_VOLTAGE
	{ .start = 52, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE1_OVER_TEMPERATURE
	{ .start = 51, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE1_FAN_FAULT
	{ .start = 50, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE1_MDL_PROTECTED
	{ .start = 49, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE1_MDL_FAULT
	{ .start = 48, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE1_MDL_OFF_STATE
	{ .start = 61, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE0_DISCHARGE_ABNORMAL
	{ .start = 59, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE0_PFC_SIDE_ABNORMAL
	{ .start = 58, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE0_INNER_COMM
	{ .start = 56, .len =  1, .msg_id = CAN_MSG_REC0_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC0_STATE0_OUTPUT_SHORT
	{ .start =  0, .len = 64, .msg_id = CAN_MSG_REC1_ON_OFF_REQUEST       },		// CAN_SIG_REC1_ON_OFF
	{ .start =  0, .len = 32, .msg_id = CAN_MSG_REC1_OUTPUT_REQUEST       },		// CAN_SIG_REC1_VOLTAGE_REQUEST
	{ .start = 32, .len = 32, .msg_id = CAN_MSG_REC1_OUTPUT_REQUEST       },		// CAN_SIG_REC1_CURRENT_REQUEST
	{ .start = 32, .len = 32, .msg_id = CAN_MSG_REC1_OUTPUT_INFO_RESPONSE },		// CAN_SIG_REC1_CURRENT_PRESENT
	{ .start =  0, .len = 32, .msg_id = CAN_MSG_REC1_OUTPUT_INFO_RESPONSE },		// CAN_SIG_REC1_VOLTAGE_PRESENT
	{ .start = 32, .len =  8, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_TEMPERATURE
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_GROUP
	{ .start = 41, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE2_MDL_ID_REPETITION
	{ .start = 47, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE2_MDL_PFC_SIDE_OFF
	{ .start = 46, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE2_INPUT_OVER_VOLTAGE
	{ .start = 45, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE2_INPUT_UNDER_VOLTAGE
	{ .start = 44, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE2_INPUT_UNBALANCE
	{ .start = 43, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE2_INPUT_PHASE_LOST
	{ .start = 42, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE2_LOAD_UNSHARING
	{ .start = 40, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE2_POWER_LIMIT
	{ .start = 55, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE1_COMM_INTERRUPT
	{ .start = 54, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE1_WALK_IN_ENABLE
	{ .start = 53, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE1_OUTPUT_OVER_VOLTAGE
	{ .start = 52, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE1_OVER_TEMPERATURE
	{ .start = 51, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE1_FAN_FAULT
	{ .start = 50, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE1_MDL_PROTECTED
	{ .start = 49, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE1_MDL_FAULT
	{ .start = 48, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE1_MDL_OFF_STATE
	{ .start = 61, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE0_DISCHARGE_ABNORMAL
	{ .start = 59, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE0_PFC_SIDE_ABNORMAL
	{ .start = 58, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE0_INNER_COMM
	{ .start = 56, .len =  1, .msg_id = CAN_MSG_REC1_ERROR_INFO_RESPONSE  },		// CAN_SIG_REC1_STATE0_OUTPUT_SHORT
	{ .start = 56, .len =  8, .msg_id = CAN_MSG_FAN_FAILDEM               },		// CAN_SIG_FAN7_FAILDEM
	{ .start = 48, .len =  8, .msg_id = CAN_MSG_FAN_FAILDEM               },		// CAN_SIG_FAN6_FAILDEM
	{ .start = 40, .len =  8, .msg_id = CAN_MSG_FAN_FAILDEM               },		// CAN_SIG_FAN5_FAILDEM
	{ .start = 32, .len =  8, .msg_id = CAN_MSG_FAN_FAILDEM               },		// CAN_SIG_FAN4_FAILDEM
	{ .start = 24, .len =  8, .msg_id = CAN_MSG_FAN_FAILDEM               },		// CAN_SIG_FAN3_FAILDEM
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_FAN_FAILDEM               },		// CAN_SIG_FAN2_FAILDEM
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_FAN_FAILDEM               },		// CAN_SIG_FAN1_FAILDEM
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_FAN_FAILDEM               },		// CAN_SIG_FAN0_FAILDEM
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_FAN_TEMP_FAILDEM          },		// CAN_SIG_TEMP2_FAILDEM
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_FAN_TEMP_FAILDEM          },		// CAN_SIG_TEMP1_FAILDEM
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_FAN_TEMP_FAILDEM          },		// CAN_SIG_TEMP0_FAILDEM
	{ .start = 56, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP0           },		// CAN_SIG_FAN3_SPEED
	{ .start = 48, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP0           },		// CAN_SIG_FAN3_DUTY
	{ .start = 40, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP0           },		// CAN_SIG_FAN2_SPEED
	{ .start = 32, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP0           },		// CAN_SIG_FAN2_DUTY
	{ .start = 24, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP0           },		// CAN_SIG_FAN1_SPEED
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP0           },		// CAN_SIG_FAN1_DUTY
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP0           },		// CAN_SIG_FAN0_SPEED
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP0           },		// CAN_SIG_FAN0_DUTY
	{ .start = 56, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP1           },		// CAN_SIG_FAN7_SPEED
	{ .start = 48, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP1           },		// CAN_SIG_FAN7_DUTY
	{ .start = 40, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP1           },		// CAN_SIG_FAN6_SPEED
	{ .start = 32, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP1           },		// CAN_SIG_FAN6_DUTY
	{ .start = 24, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP1           },		// CAN_SIG_FAN5_SPEED
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP1           },		// CAN_SIG_FAN5_DUTY
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP1           },		// CAN_SIG_FAN4_SPEED
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_FAN_INFO_GROUP1           },		// CAN_SIG_FAN4_DUTY
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_FAN_TEMP_INFO             },		// CAN_SIG_TEMP2_INFO
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_FAN_TEMP_INFO             },		// CAN_SIG_TEMP1_INFO
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_FAN_TEMP_INFO             },		// CAN_SIG_TEMP0_INFO
	{ .start = 48, .len = 16, .msg_id = CAN_MSG_FAN_VERSION               },		// CAN_SIG_FAN_HW_VERSION_SUBMINOR
	{ .start = 40, .len =  8, .msg_id = CAN_MSG_FAN_VERSION               },		// CAN_SIG_FAN_HW_VERSION_MAJOR
	{ .start = 24, .len =  8, .msg_id = CAN_MSG_FAN_VERSION               },		// CAN_SIG_FAN_VESION_BOOT_SUBMINOR
	{ .start = 20, .len =  4, .msg_id = CAN_MSG_FAN_VERSION               },		// CAN_SIG_FAN_VERSION_BOOT_MINOR
	{ .start = 16, .len =  4, .msg_id = CAN_MSG_FAN_VERSION               },		// CAN_SIG_FAN_VERSION_BOOT_MAJOR
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_FAN_VERSION               },		// CAN_SIG_FAN_VESION_APP_SUBMINOR
	{ .start =  4, .len =  4, .msg_id = CAN_MSG_FAN_VERSION               },		// CAN_SIG_FAN_VERSION_APP_MINOR
	{ .start =  0, .len =  4, .msg_id = CAN_MSG_FAN_VERSION               },		// CAN_SIG_FAN_VESION_APP_MAJOR
	{ .start = 32, .len =  8, .msg_id = CAN_MSG_FAN_VERSION               },		// CAN_SIG_FAN_HW_VERSION_MINOR
	{ .start = 24, .len =  8, .msg_id = CAN_MSG_FAN_SYSTEM_FAILDEM        },		// CAN_SIG_FAN_SYSTEM2_FAILDEM_ALL_TEMPS
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_FAN_SYSTEM_FAILDEM        },		// CAN_SIG_FAN_SYSTEM1_FAILDEM_ALL_TEMPS
	{ .start = 56, .len =  8, .msg_id = CAN_MSG_FAN_SYSTEM_FAILDEM        },		// CAN_SIG_FAN_SYSTEM2_COOLING_DONE
	{ .start = 48, .len =  8, .msg_id = CAN_MSG_FAN_SYSTEM_FAILDEM        },		// CAN_SIG_FAN_SYSTEM1_COOLING_DONE
	{ .start = 40, .len =  8, .msg_id = CAN_MSG_FAN_SYSTEM_FAILDEM        },		// CAN_SIG_FAN_SYSTEM2_OVER_TEMP
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_FAN_SYSTEM_FAILDEM        },		// CAN_SIG_FAN_SYSTEM2_FAILDEM_ALL_FANS
	{ .start = 32, .len =  8, .msg_id = CAN_MSG_FAN_SYSTEM_FAILDEM        },		// CAN_SIG_FAN_SYSTEM1_OVER_TEMP
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_FAN_SYSTEM_FAILDEM        },		// CAN_SIG_FAN_SYSTEM1_FAILDEM_ALL_FANS
	{ .start = 32, .len = 32, .msg_id = CAN_MSG_REC0_OUTPUT_NOTIFY        },		// CAN_SIG_REC0_CURRENT_NOTIFY
	{ .start =  0, .len = 32, .msg_id = CAN_MSG_REC0_OUTPUT_NOTIFY        },		// CAN_SIG_REC0_VOLTAGE_NOTIFY
	{ .start =  0, .len = 32, .msg_id = CAN_MSG_REC1_OUTPUT_NOTIFY        },		// CAN_SIG_REC1_VOLTAGE_NOTIFY
	{ .start = 32, .len = 32, .msg_id = CAN_MSG_REC1_OUTPUT_NOTIFY        },		// CAN_SIG_REC1_CURRENT_NOTIFY
	{ .start = 28, .len =  4, .msg_id = CAN_MSG_SYSTEM_MODE               },		// CAN_SIG_AC_RELAY_REQUEST
	{ .start = 24, .len =  4, .msg_id = CAN_MSG_SYSTEM_MODE               },		// CAN_SIG_SYSTEM_STATE
	{ .start = 16, .len =  8, .msg_id = CAN_MSG_SYSTEM_MODE               },		// CAN_SIG_V2G1_STATE
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_SYSTEM_MODE               },		// CAN_SIG_V2G0_STATE
	{ .start =  4, .len =  4, .msg_id = CAN_MSG_SYSTEM_MODE               },		// CAN_SIG_FAST_CHARGE
	{ .start =  0, .len =  4, .msg_id = CAN_MSG_SYSTEM_MODE               },		// CAN_SIG_ACTIVE_OUTLET
	{ .start = 48, .len = 16, .msg_id = CAN_MSG_SYSTEM_OUTPUT_REQUEST     },		// CAN_SIG_DC1_VOLTAGE_MAX
	{ .start = 32, .len = 16, .msg_id = CAN_MSG_SYSTEM_OUTPUT_REQUEST     },		// CAN_SIG_DC0_VOLTAGE_MAX
	{ .start = 16, .len = 16, .msg_id = CAN_MSG_SYSTEM_OUTPUT_REQUEST     },		// CAN_SIG_DC1_VOLTAGE_REQUEST
	{ .start =  0, .len = 16, .msg_id = CAN_MSG_SYSTEM_OUTPUT_REQUEST     },		// CAN_SIG_DC0_VOLTAGE_REQUEST
	{ .start = 33, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_EXTMEM_FAIL
	{ .start = 56, .len =  4, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_SELFTEST_STATE
	{ .start = 11, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_AC_RELAY_STATUS
	{ .start = 48, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT_LV_RELAY_ERROR
	{ .start = 47, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT_HV_RELAY_ERROR
	{ .start = 46, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT_PRE_RELAY_ERROR
	{ .start = 45, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT1_LV_RELAY_ERROR
	{ .start = 44, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT1_HV_RELAY_ERROR
	{ .start = 43, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT1_PRE_RELAY_ERROR
	{ .start = 42, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT0_LV_RELAY_ERROR
	{ .start = 41, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT0_HV_RELAY_ERROR
	{ .start = 40, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT0_PRE_RELAY_ERROR
	{ .start = 32, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OVERHEAT
	{ .start = 31, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_ISO_N1_ERROR
	{ .start = 30, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_ISO_P1_ERROR
	{ .start = 29, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_ISO_N0_ERROR
	{ .start = 28, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_ISO_P0_ERROR
	{ .start = 26, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT1_ABNORMAL
	{ .start = 25, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT1_UNDER_VOLTAGE
	{ .start = 24, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT1_OVER_VOLTAGE
	{ .start = 18, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT0_ABNORMAL
	{ .start = 17, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT0_UNDER_VOLTAGE
	{ .start = 16, .len =  1, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_OUTPUT0_OVER_VOLTAGE
	{ .start = 12, .len =  3, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_INPUT_UNDER_VOLTAGE
	{ .start =  8, .len =  3, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_INPUT_OVER_VOLTAGE
	{ .start =  4, .len =  4, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_STATUS1
	{ .start =  0, .len =  4, .msg_id = CAN_MSG_HV_ALERT_STATUS           },		// CAN_SIG_HV_STATUS0
	{ .start = 16, .len =  4, .msg_id = CAN_MSG_HV_MONITOR                },		// CAN_SIG_HV_CHARGER_MODE
	{ .start = 60, .len =  4, .msg_id = CAN_MSG_HV_MONITOR                },		// CAN_SIG_DC1_PRECHARGE_RELAY
	{ .start = 56, .len =  4, .msg_id = CAN_MSG_HV_MONITOR                },		// CAN_SIG_DC1_MAIN_RELAY
	{ .start = 52, .len =  4, .msg_id = CAN_MSG_HV_MONITOR                },		// CAN_SIG_DC1_ISOLATION_STATUS
	{ .start = 32, .len = 16, .msg_id = CAN_MSG_HV_MONITOR                },		// CAN_SIG_DC1_VOLTAGE_PRESENT
	{ .start = 28, .len =  4, .msg_id = CAN_MSG_HV_MONITOR                },		// CAN_SIG_DC0_PRECHARGE_RELAY
	{ .start = 24, .len =  4, .msg_id = CAN_MSG_HV_MONITOR                },		// CAN_SIG_DC0_MAIN_RELAY
	{ .start = 20, .len =  4, .msg_id = CAN_MSG_HV_MONITOR                },		// CAN_SIG_DC0_ISOLATION_STATUS
	{ .start =  0, .len = 16, .msg_id = CAN_MSG_HV_MONITOR                },		// CAN_SIG_DC0_VOLTAGE_PRESENT
	{ .start = 24, .len =  8, .msg_id = CAN_MSG_HV_VERSION                },		// CAN_SIG_HV_VERSION_BOOT_SUBMINOR
	{ .start = 20, .len =  4, .msg_id = CAN_MSG_HV_VERSION                },		// CAN_SIG_HV_VERSION_BOOT_MINOR
	{ .start = 16, .len =  4, .msg_id = CAN_MSG_HV_VERSION                },		// CAN_SIG_HV_VERSION_BOOT_MAJOR
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_HV_VERSION                },		// CAN_SIG_HV_VERSION_APP_SUBMINOR
	{ .start =  4, .len =  4, .msg_id = CAN_MSG_HV_VERSION                },		// CAN_SIG_HV_VERSION_APP_MINOR
	{ .start =  0, .len =  4, .msg_id = CAN_MSG_HV_VERSION                },		// CAN_SIG_HV_VERSION_APP_MAJOR
	{ .start = 32, .len =  4, .msg_id = CAN_MSG_HEART_BEAT                },		// CAN_SIG_SYSTEM_REBOOT
	{ .start = 28, .len =  4, .msg_id = CAN_MSG_HEART_BEAT                },		// CAN_SIG_CONFIGURE_CHARGER_MODE
	{ .start = 24, .len =  4, .msg_id = CAN_MSG_HEART_BEAT                },		// CAN_SIG_COMM_REQUEST_XMC1
	{ .start = 20, .len =  4, .msg_id = CAN_MSG_HEART_BEAT                },		// CAN_SIG_COMM_REQUEST_XMC0
	{ .start = 56, .len =  8, .msg_id = CAN_MSG_HEART_BEAT                },		// CAN_SIG_COMM_REQUEST_ROLL_COUNTER
	{ .start =  8, .len =  4, .msg_id = CAN_MSG_HEART_BEAT                },		// CAN_SIG_COMM_REQUEST_EXT
	{ .start =  4, .len =  4, .msg_id = CAN_MSG_HEART_BEAT                },		// CAN_SIG_COMM_REQUEST_HV
	{ .start =  0, .len =  4, .msg_id = CAN_MSG_HEART_BEAT                },		// CAN_SIG_COMM_REQUEST_FAN
	{ .start =  8, .len =  8, .msg_id = CAN_MSG_CP0_MONITOR               },		// CAN_SIG_MONITOR_PE0_LOST
	{ .start =  0, .len =  8, .msg_id = CAN_MSG_CP0_MONITOR               },		// CAN_SIG_MONITOR_CP0_LOST
};
