all: jni plugin

install: jni-install plugin-install
	/etc/init.d/geoserver restart

jni:
	cd ams/jni/build ; cmake ../ ; make

plugin:
	cd geoserver/plugin/ ; mvn install

jni-install:
	cp ams/jni/build/libacid-maps-jni.so /usr/lib
	ldconfig

plugin-install:
	cp geoserver/plugin/target/acidmaps-1.0.jar /opt/geoserver/webapps/geoserver/WEB-INF/lib/
