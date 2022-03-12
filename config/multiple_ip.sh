cp config/multiple_ip_tmp.conf multiple_ip.conf
Cluster_IP=$(ip a | grep -o "[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}" | sed 2!d)
sed -ie "s/@clusterIP/$Cluster_IP/g" multiple_ip.conf
rm multiple_ip.confe