sudo sysctl -w kernel.randomize_va_space=0
sudo ln -sf /bin/zsh /bin/sh

sudo su
gcc -m32 -o a2 -z execstack -fno-stack-protector A2.c
sudo chown root a2
sudo chmod 4755 a2
su seed


rm -f badfile
touch badfile
gcc -m32 -o g_a2 -g -z execstack -fno-stack-protector A2.c

