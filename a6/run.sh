./soda soda.config 62017 > out &
sleep 0.2 ; kill $!
less out
