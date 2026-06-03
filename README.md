Experiment related changes to be made:

If can't compile check platformio.io some arduinos are /dev/ttyUSB0 /dev/ttyACM0

change main.cpp
elab:
```
    expr.begin("ELAB_FIVEPOLARIZER");
```

oeiras:
```
    expr.begin("OEIRAS_FIVEPOLARIZER");
```

change user_define.cpp directions and vrefmode
elab:
```
    uint8_t resetDir = HIGH;
    uint8_t dirToTop = LOW;
    int vrefMode = 1;
```

oeiras:
```
    uint8_t resetDir = HIGH;
    uint8_t dirToTop = LOW;
    int vrefMode = 3;
```

lordelos order of steppers is switched:
```
Stepper stepperArray[5] = {stepper_E0,stepper_Z,stepper_E1,stepper_Y,stepper_X};
Switch switchArray[5] = {switch_E0, switch_Z, switch_E1, switch_Y, switch_X};
```

How to setup rc-local:
sudo nano /etc/rc.local
```
_IP=$(hostname -I) || true
if [ "$_IP" ]; then
  printf "My IP address is %s\n" "$_IP"
fi
sleep 120
cd /home/elab/RPi_Proxy_fivepol
su elab -c "sh start-wp.sh &"
exit 0
```
create systemctl service
