#include <iostream>
#include <modbus/modbus.h>
#include <unistd.h>



modbus_t *ctx;
void csetrts(modbus_t *ctx, int on);


void SendData(uint16_t data, uint16_t reg_nr)
{
 ctx = modbus_new_rtu("/dev/serial0", 115200, 'N', 8, 1);
 modbus_rtu_set_serial_mode(ctx,MODBUS_RTU_RS485);
 modbus_rtu_set_rts(ctx, MODBUS_RTU_RTS_UP);
 modbus_set_error_recovery(ctx, /*MODBUS_ERROR_RECOVERY_LINK |*/ MODBUS_ERROR_RECOVERY_PROTOCOL);
 modbus_rtu_set_custom_rts(ctx, csetrts);
 modbus_set_slave(ctx, 0x0A);
 modbus_connect(ctx);
 modbus_set_debug(ctx,1);
 modbus_flush(ctx);
if(1!= modbus_write_register(ctx,reg_nr,data))
 {
   usleep(1000);
   modbus_flush(ctx);
   if(1!= modbus_write_register(ctx,reg_nr,data)){exit(0);}
 }
 modbus_free(ctx);
 modbus_close(ctx);

}


void csetrts(modbus_t *ctx, int on){
FILE *fp;
fp = fopen("/sys/class/gpio/gpio17/value", "w+");
fprintf(fp,"%d",on);
fclose(fp);
fp = fopen("/sys/class/gpio/gpio16/value", "w+");
fprintf(fp,"%d",on);
fclose(fp);
}



int main()
{

for(int i=0;i<0xFFF;i++)
{
 SendData(i,8);
 SendData(i,9);
}

}
