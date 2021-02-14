#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "bsp.h"
#include "nrf_atfifo.h"
#include "app_button.h" 
#include "app_timer.h"
#include "app_error.h"
#include "nrf_drv_clock.h"
#include "led_softblink.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/* !!!! MIGHT WANT TO USE THESE !!!! */
#define BUTTON_DEBOUNCE_DELAY       50
#define TIMEOUT_DELAY               APP_TIMER_TICKS(10000)






/* Define Timer */
APP_TIMER_DEF(timeout_timer);


/* Function pointer primitive */ 
typedef void (*state_func_t)( void );

struct _state 
{
    uint8_t id;
    state_func_t Enter;
    state_func_t Do;
    state_func_t Exit;
    uint32_t delay_ms;
};
typedef struct _state state_t; 



enum _event 
{
    b1_evt = 0,
    b2_evt = 1,
    b3_evt = 2,
    timeout_evt = 3,
    no_evt = 4
};
typedef enum _event event_t; 


/* !!!! PART 2 & 3 */
/* Define FIFO */
NRF_ATFIFO_DEF(event_fifo, event_t, 10);



static void button_handler(uint8_t pin_num, uint8_t btn_action)
{
    event_t evt;

    if(btn_action == APP_BUTTON_PUSH)
    {
		NRF_LOG_INFO("button pressed");	
        switch(pin_num)
        {
            case BUTTON_1:
                evt = b1_evt;
                nrf_atfifo_alloc_put(event_fifo, &evt, sizeof(event_t), NULL);
				NRF_LOG_INFO("button 1 pressed");	
            break;

            case BUTTON_2:
                evt = b2_evt;
                nrf_atfifo_alloc_put(event_fifo, &evt, sizeof(event_t), NULL);
				NRF_LOG_INFO("button 2 pressed");
            break;
			
			case BUTTON_3:
                evt = b3_evt;
                nrf_atfifo_alloc_put(event_fifo, &evt, sizeof(event_t), NULL);
				NRF_LOG_INFO("button 3 pressed");
            break;
        }
    }
}

static const app_button_cfg_t p_buttons[] = {
        {BUTTON_1, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_handler},
        {BUTTON_2, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_handler},
		{BUTTON_3, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_handler}
};


static void timeout_handler(void * p_context)
{	event_t evt = timeout_evt;
    nrf_atfifo_alloc_put(event_fifo, &evt, sizeof(event_t), NULL);
}

static led_sb_init_params_t fading = {
            .active_high        = LED_SB_INIT_PARAMS_ACTIVE_HIGH,
            .duty_cycle_max     = LED_SB_INIT_PARAMS_DUTY_CYCLE_MAX,
            .duty_cycle_min     = LED_SB_INIT_PARAMS_DUTY_CYCLE_MIN,
            .duty_cycle_step    = 1,
            .off_time_ticks     = APP_TIMER_TICKS(5000),
            .on_time_ticks      = APP_TIMER_TICKS(5000),
            .leds_pin_bm        = LED_SB_INIT_PARAMS_LEDS_PIN_BM(LEDS_MASK),
            .p_leds_port        = LED_SB_INIT_PARAMS_LEDS_PORT
        };

/*static led_sb_init_params_t fading ={
	false, 
	220, 
	0, 
	5,
	65000,
	65000,
	4,
	NRF_GPIO
};

//LED_SB_INIT_DEFAULT_PARAMS(LEDS_MASK);


*/

void init_board(void)
{
    /* Initialize the low frequency clock used by APP_TIMER */
    uint32_t err_code;
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    NRF_LOG_INFO("Logging initialized.");

    /* Initialize the event fifo */
    /* !!!! PART 2 !!!! */
    NRF_ATFIFO_INIT(event_fifo);


    /* Initialize the timer module */ 
    /* !!!! PART 2 !!!! */
    app_timer_init();
    app_timer_create(&timeout_timer, APP_TIMER_MODE_SINGLE_SHOT, timeout_handler);
	
    /* Initialize the LEDs */
	bsp_board_init(BSP_INIT_LEDS);
    
    /* Setup button interrupt handler */
    /* !!!! PART 2 !!!! */
	app_button_init(p_buttons, ARRAY_SIZE(p_buttons), BUTTON_DEBOUNCE_DELAY);
    app_button_enable();

    /* Initialize fading LED driver */ 
    /* !!!! PART 3 !!!! */
	err_code = led_softblink_init(&fading);
    APP_ERROR_CHECK(err_code);

}


event_t get_event(void){
    event_t evt = no_evt;
    /* In case the fifo is empty, nothing will be written to &evt*
	* This function gets the value from the top of the FIFO. */
	//NRF_LOG_INFO("in get event");	
    nrf_atfifo_get_free(event_fifo, &evt, sizeof(event_t), NULL);
    return evt; 
}

static int clock_wise[4]={0,1,3,2};
static int c_clock_wise[4]={0,2,3,1};




void enter_state_0(void)
{
    /* !!!! PART 1 !!!! */
	NRF_LOG_INFO("state 0 entered")
	return;
}

void exit_state_0(void)
{
    /* !!!! PART 1 !!!! */
	//bsp_board_leds_off();
	NRF_LOG_INFO("state 0 entered");
}

/* !!!! PART 1 !!!! */


void enter_state_1(void)
{
    /* !!!! PART 1 !!!! */
	//bsp_board_leds_off();
	NRF_LOG_INFO("state 1 entered");
	return;
}

void enter_state_2(void)
{
    /* !!!! PART 1 !!!! */
	//bsp_board_leds_off();
	NRF_LOG_INFO("state 2 entered");
	return;
}

void enter_state_3(void)
{
    /* !!!! PART 1 !!!! */
	//bsp_board_leds_off();
	NRF_LOG_INFO("state 3 entered");
	return;
}

void exit_state_1(void)
{
    /* !!!! PART 1 !!!! */
	NRF_LOG_INFO("state 1 exited");
	return;
}

void exit_state_2(void)
{
    /* !!!! PART 1 !!!! */	
	NRF_LOG_INFO("state 2 exited")
	return;
}

void exit_state_3(void)
{
    /* !!!! PART 1 !!!! */
	led_softblink_stop();
	NRF_LOG_INFO("state 3 exited")
	return;
}



void do_state_0(void)
{
    /* !!!! PART 1 !!!! */
	static int led_num;
	bsp_board_leds_off();
	bsp_board_led_on(clock_wise[led_num]);
	led_num++;	
	led_num = led_num % 4;
}

void do_state_1(void)
{
    /* !!!! PART 1 !!!! */
	bsp_board_leds_off();
	nrf_delay_ms(500);            
	bsp_board_leds_on();
}


void do_state_2(void)
{
    /* !!!! PART 1 !!!! */
	static int led_num;
	bsp_board_leds_off();
	bsp_board_led_on(c_clock_wise[led_num]);
	led_num++;	
	led_num = led_num % 4;
}

void do_state_3(void)
{
    /* !!!! PART 3 !!!! */
	
	led_softblink_start(LEDS_MASK);
	app_timer_start(timeout_timer, TIMEOUT_DELAY, NULL);
	
	
	
}

/* !!!! PART 1 !!!! */

const state_t state0 = {
    0, 
    enter_state_0,
    do_state_0,
    exit_state_0, 
    200
};

const state_t state1 = {
    1, 
    enter_state_1,
    do_state_1,
    exit_state_1, 
    200
};



const state_t state2 = {
    2, 
    enter_state_2,
    do_state_2,
    exit_state_2, 
    200
};

const state_t state3 = {
    3, 
    enter_state_3,
    do_state_3,
    exit_state_3, 
    200
};

/* !!!! PART 2 !!!! */
const state_t state_table[6][5] = {
	///*  STATE       	  b1_evt     b2_evt      b3_evt		 timeout_evt     no_evt */   
	{/* state0 */    	  state2,    state1,     state3,	 state0,		 state0},
    {/* state1 */   	  state0,    state2,     state3,     state1,		 state1},    
    {/* state2 */   	  state1,    state0,     state3,     state2,		 state2}, 
	{/* state3 */   	  state3,    state3,     state3,     state0,		 state3}, 
};

/* !!!! FOR ALL PARTS !!!! */
int main(void)
{
    NRF_LOG_INFO("In main");
    init_board();
	event_t evt;

    state_t current_state = state0;
	evt = no_evt;
    

    for(;;)
    {	current_state = state_table[current_state.id][evt];
        current_state.Enter();
        //evt = get_event();

        while(1)
        {
            current_state.Do();            
			evt = get_event();
			nrf_delay_ms(current_state.delay_ms);		
            NRF_LOG_FLUSH();
			if (evt != no_evt)
				break;			
        }
        current_state.Exit();
		__WFE();
		NRF_LOG_INFO("current state exited");	
    
    }
    
}
