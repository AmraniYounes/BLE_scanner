#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>


static void init_callback(int err);
static void scan_callback(const bt_addr_le_t *, int8_t,
                          uint8_t, struct net_buf_simple *);


const struct bt_le_scan_param scan_params = {
    .type = BT_LE_SCAN_TYPE_ACTIVE,
    .options = BT_LE_SCAN_OPT_NONE,
    .interval   = 0x0010,
	.window     = 0x0010,
};

static uint8_t mfg_data[] = { 0xff, 0xff, 0x00 };
static const struct bt_data ad[] = {
	BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 3),
};

int main(void)
{
    // Set public address for Bluetooth controller
    const uint8_t public_address[6] = {0xFF, 0xFF, 0xFF, 0x00, 0xEE, 0xEE};
    bt_ctlr_set_public_addr(public_address);	

    // init BLE
    int err = bt_enable(NULL);
    if(err)
    {
        printk("Bluetooth init failed (error code: %d)\n", err);
        return -1;
    }
    

    err = bt_le_scan_start(&scan_params, scan_callback);
    if(err)
    {
        printk("Scanning failed (error code: %d)\n", err);
        return -1;
    }

    

    // 2nd task: log device data
    // make sure then scan and log are periodic
    // deinit the BLE when a button is pressed on the esp32

    return 0;
}




static void scan_callback(const bt_addr_le_t *public_address, int8_t rssi, uint8_t adv_type
                                                                , struct net_buf_simple *buf)
{
    mfg_data[2]++;
}
