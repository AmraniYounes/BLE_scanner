#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/controller.h>
#include <zephyr/bluetooth/addr.h>


static void init_callback(int err);
static void scan_callback(const bt_addr_le_t *, int8_t,
                          uint8_t, struct net_buf_simple *);


const struct bt_le_scan_param obs_scan_params = {
    .type = BT_LE_SCAN_TYPE_ACTIVE,
    .options = BT_LE_SCAN_OPT_NONE,
    .interval = 0x0060,
    .window = 0x0030
};




int main(void)
{
    // Set public address for Bluetooth controllera
    const uint8_t public_address[6] = {0xFF, 0xFF, 0xFF, 0x00, 0xEE, 0xEE};
    bt_ctlr_set_public_addr(public_address);	

    // init BLE
    int err = bt_enable(init_callback);

    

    // 2nd task: log device data
    // make sure then scan and log are periodic
    // deinit the BLE when a button is pressed on the esp32

    return 0;
}



static void init_callback(int err)
{
    if(err)
    {
        printk("Bluetooth init faild (err %d)\n", err);
    }
    printk("Bluetooth is ready!\n");

    // 1st task: scan network if esp32 button is pressed
    if(bt_le_scan_start(&obs_scan_params, scan_callback) != 0)
    {
        printk("Error starting the observer scan!\n");
    }
}

static void scan_callback(const bt_addr_le_t *public_address, int8_t rssi, uint8_t adv_type
                                                                , struct net_buf_simple *buf)
{
    char addr_str[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(public_address, addr_str, sizeof(addr_str));
    printk("Device found: %s (RSSI %d)\n", addr_str, rssi);
}
