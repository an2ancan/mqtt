cmake -Bbuild -H.
cmake --build build/ --target install && ldconfig 
chmod +x build/src/publisher/publisher
chmod +x build/src/subscriber/subscriber