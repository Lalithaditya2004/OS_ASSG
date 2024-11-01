## Create a Makefile with the following commands
```
obj-m += mgdev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

```

## Compile the Module
Run make in the directory where mgdev.c and Makefile are located.
```
make
```

## Load the Module
To load the module into the kernel, use the insmod command:
```
sudo insmod mgdev.ko kernel_version=6,11,2
```
Upon loading, the kernel will print a message confirming that the device has been created. You can view this message by running:
```
dmesg | tail -n 10
```

## Create the device file
Note down the major number which is shown in the kernel log when we ran the previous command.
Use this major number to create device file using command.
```
sudo mknod /dev/mgdev c <major_number> 0
```

## Change device file permissions
After creating the device file, update its permissions to allow read and write access.
Use chmod command to do so.
```
sudo chmod 666 /dev/mgdev
```

## Interact with the device
Once the device file exists, you can test reading from and writing to it.
Writing to the Device:
```
echo "KAJA_B220937CCS" > /dev/mgdev
```
Reading from the Device:
```
cat /dev/mgdev
```

## Unload the Module
To remove the module from the kernel, use rmmod:
```
sudo rmmod mgdev
```

## Remove the Device File
After unloading the module, delete the device file from /dev:
```
sudo rm /dev/mgdev
```

## Clean Up
To remove compiled files, use:
```
make clean
```





