#include "SITLSensors.hpp"
#include "log_instance_pool.hpp"


LOG_INSTANCE_POOL_DEFINE(sitl_sensors_driver, 1)


SITLSensors::SITLSensors():
	ITask(taskConf)
{
	init_logging(sitl_sensors_driver_log_instance_get(0));
}

SITLSensors::~SITLSensors() = default;


bool SITLSensors::init()
{
	if(sim_socket_.init(CONFIG_AUTOPILOT_PORT, "127.0.0.1", CONFIG_SIM_PORT)){
		log_inf("Socket inizializzato correttamente!");
	}else{
		log_wrn("Errore nell'inizializzazione del socket !");
	}

	log_inf("SITL-Sensors initialized!");
	mavlink_message_t msg;
	mavlink_msg_heartbeat_pack(
		1, MAV_COMP_ID_AUTOPILOT1, &msg,
		MAV_TYPE_QUADROTOR,
		MAV_AUTOPILOT_GENERIC,
		MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,
		0,
		MAV_STATE_ACTIVE);
	
	send_mavlink_msg(msg);
    return true;
}

void SITLSensors::run(){
	uint8_t rx_buff[MAVLINK_MAX_PACKET_LEN];
	while (true)
	{
		uint64_t now = k_uptime_get();

		// Invio heartbeat periodico al sim
		if (now - last_heartbeat_ >= 1000)
		{
			mavlink_message_t msg;
			mavlink_msg_heartbeat_pack(
				1, MAV_COMP_ID_AUTOPILOT1, &msg,
				MAV_TYPE_QUADROTOR,
				MAV_AUTOPILOT_GENERIC,
				MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,
				0,
				MAV_STATE_ACTIVE);
			
			last_heartbeat_ = now;
			send_mavlink_msg(msg);
		}
		


		int len = sim_socket_.recive(rx_buff, sizeof(rx_buff));

		if (len > 0)
		{
			mavlink_message_t msg;
            mavlink_status_t status;
            for (int i = 0; i < len; ++i) {
                if (mavlink_parse_char(MAVLINK_COMM_0, rx_buff[i], &msg, &status)) {
                    handle_mavlink_msg(msg);
                }
            }

		}

		mavlink_message_t actuators_commands;

		float controls[16] = {0.0f}; 
		// Esempio: imposta i primi 4 canali con valori tra -1.0 e 1.0
		controls[0] = 0.0f;
		controls[1] = 0.0f;
		controls[2] = 0.0f;
		controls[3] = 0.0f;
		mavlink_msg_hil_actuator_controls_pack(
			1, MAV_COMP_ID_AUTOPILOT1, &actuators_commands,
			k_uptime_get(), controls, MAV_MODE_FLAG_SAFETY_ARMED, 0
		);

		send_mavlink_msg(actuators_commands);

		
		k_yield();
	}
	
}

void SITLSensors::send_mavlink_msg(mavlink_message_t& msg){
	uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    int len = mavlink_msg_to_send_buffer(buffer, &msg);
    if (len > 0) {
        sim_socket_.send(buffer, len);
    }
}


void SITLSensors::handle_mavlink_msg(mavlink_message_t& msg){
	switch (msg.msgid)
	{
	case MAVLINK_MSG_ID_HIL_SENSOR:
		mavlink_hil_sensor_t sensors_msg;
		mavlink_msg_hil_sensor_decode(&msg, &sensors_msg);

		handle_hil_sensor(sensors_msg);
		break;
	
	case MAVLINK_MSG_ID_HIL_GPS:
		break;
	
	default:
		break;
	}
}


void SITLSensors::handle_hil_sensor(mavlink_hil_sensor_t& sensors_msg){
	
	uint64_t timestamp = sensors_msg.time_usec;

	// 2. FAST-FORWARD DEL TEMPO SIMULATO
	if (last_sim_time_ > 0 && timestamp > last_sim_time_) {
	    uint32_t dt_us = timestamp - last_sim_time_;
	
	    // MAGIA: native_sim non aspetta tempo reale.
	    // Avanza il clock globale di Zephyr di 'dt' microsecondi all'istante.
	    // Se l'EKF o i PID hanno dei k_sleep in attesa, si eseguiranno ora!
	    k_sleep(K_USEC(dt_us)); 
	}
	last_sim_time_ = timestamp;

	log_inf("MSG_HIL_SENSOR => xacc: %f | yacc: %f | zacc: %f", sensors_msg.xacc, sensors_msg.yacc, sensors_msg.zacc);
}


void SITLSensors::handle_hil_gps(){

}
