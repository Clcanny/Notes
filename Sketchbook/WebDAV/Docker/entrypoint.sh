#!/bin/bash

mv /etc/nginx/conf.d/default.conf.unused /etc/nginx/conf.d/default.conf
certbot certonly --standalone -d viviansj520.cn -d www.viviansj520.cn
if [[ -n "$USERNAME" ]] && [[ -n "$PASSWORD" ]]
then
	htpasswd -bc /etc/nginx/htpasswd $USERNAME $PASSWORD
	echo Done.
else
    echo Using no auth.
	sed -i 's%auth_basic "Restricted";% %g' /etc/nginx/conf.d/default.conf
	sed -i 's%auth_basic_user_file htpasswd;% %g' /etc/nginx/conf.d/default.conf
fi
