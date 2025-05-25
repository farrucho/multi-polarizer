Experiment related changes to be made:

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
    uint8_t resetDir = LOW;
    uint8_t dirToTop = HIGH;
    int vrefMode = 3;
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