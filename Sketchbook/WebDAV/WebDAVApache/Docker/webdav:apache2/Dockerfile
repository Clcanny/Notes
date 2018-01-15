FROM debian:9

ADD sources.list /etc/apt/sources.list
RUN apt-get update
RUN apt-get upgrade -y

RUN apt-get install -y apache2 apache2-utils
ADD 000-default.conf /etc/apache2/sites-available/000-default.conf

RUN mkdir /etc/apache2/ssl
ADD apache.crt /etc/apache2/ssl/apache.crt
ADD apache.key /etc/apache2/ssl/apache.key
# ADD default-ssl.conf /etc/apache2/sites-available/default-ssl.conf

EXPOSE 443
VOLUME /var/www/webdav

ADD entrypoint.sh /
RUN chmod +x entrypoint.sh
ADD httpd-foreground /
RUN chmod +x httpd-foreground
CMD /entrypoint.sh && /httpd-foreground
