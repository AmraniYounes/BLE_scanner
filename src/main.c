#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/addr.h>


// static void init_callback(int err);
static void scan_callback(const bt_addr_le_t *, int8_t,
                          uint8_t, struct net_buf_simple *);


const struct bt_le_scan_param scan_params = {
    .type = BT_LE_SCAN_TYPE_ACTIVE,
    .options = BT_LE_SCAN_OPT_NONE,
    .interval   = 0x0010,
	.window     = 0x0010,
};



int main(void)
{

    // init BLE
    int err = bt_enable(NULL);
    if(err)
    {
        printk("Bluetooth init failed (error code: %d)\n", err);
        return -1;
    }
    

    err = bt_le_scan_start(&scan_params, scan_callback);
    if (err)
    {
        printk("Scan start failed (%d)\n", err);
    }
    else
    {
        printk("Scanning started\n");
    }

    

    // 2nd task: log device data
    // make sure that scan and log are periodic
    // myb add a task to parse the adv packets on a thread while the other app logic runs on another thread
    // disable the BLE when a button is pressed on the esp32
    while (1)
    {
        k_sleep(K_SECONDS(10));
    }

    return 0;
}




static void scan_callback(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type
                                                                , struct net_buf_simple *buf)
{

    char addr_str[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));

    printk("Device: %s | RSSI: %d dBm | Type: %u | Data len: %u\n",
           addr_str, rssi, adv_type, buf->len);
}
