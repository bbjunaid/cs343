./soda soda.config 62017 > out &
sleep 1 ; kill $!
less out
