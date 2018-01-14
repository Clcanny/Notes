FROM debian:9

ADD sources.list /etc/apt/sources.list
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y nginx certbot apache2-utils lsof

VOLUME /data
EXPOSE 80 443
ADD webdav.conf /etc/nginx/conf.d/default.conf.unused
RUN rm /etc/nginx/sites-enabled/*

COPY entrypoint.sh /
RUN chmod +x entrypoint.sh
CMD /entrypoint.sh && nginx -g "daemon off;"
