debug: android-copy-assets
	$(Q)cd android-project && ndk-build V=1
	$(Q)cd android-project && ant debug 
	$(Q)cp android-project/bin/app-debug.apk ./app.apk	

release: android-copy-assets
	$(Q)cd android-project && ndk-build V=1
	$(Q)cd android-project && ant release 
	$(Q)cp android-project/bin/app-release-unsigned.apk ./


clean:
	$(Q)rm -rf android-project/bin
	$(Q)rm -rf android-project/obj
	$(Q)rm -rf android-project/libs
	$(Q)rm -rf android-project/gen
	$(Q)rm -rf app.apk app-relase-unsigned.apk android-project/res/drawable-*


android-copy-assets:
	$(Q)for i in hdpi ldpi mdpi xhdpi; do mkdir -p android-project/res/drawable-$${i}; cp icon.png android-project/res/drawable-$${i}/; done



