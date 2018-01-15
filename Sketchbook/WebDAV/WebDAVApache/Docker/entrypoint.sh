#!/bin/bash

a2enmod dav
a2enmod dav_fs
a2enmod ssl
a2enmod auth_digest

htdigest -c /etc/apache2/users.password webdav $USERNAME
chown -R www-data:www-data /var/www/
chown www-data:www-data /etc/apache2/users.password

service apache2 restart
service apache2 stop
