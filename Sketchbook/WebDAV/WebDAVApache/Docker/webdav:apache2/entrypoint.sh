#!/bin/bash

a2enmod dav
a2enmod dav_fs
a2enmod ssl
chown -R www-data:www-data /var/www/
# service apache2 restart

htdigest -c /etc/apache2/users.password webdav $USERNAME
chown www-data:www-data /etc/apache2/users.password
a2enmod auth_digest
service apache2 restart

# a2ensite default-ssl.conf

service apache2 stop
