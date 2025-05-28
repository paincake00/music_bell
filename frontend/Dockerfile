FROM nginx:alpine

COPY nginx.conf /etc/nginx/nginx.conf
COPY index.html /usr/share/nginx/html/index.html
COPY images/ /usr/share/nginx/html/images/

# 👇 добавляем это:
RUN chmod -R 755 /usr/share/nginx/html/images