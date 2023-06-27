curl 'https://api.twilio.com/2010-04-01/Accounts/ACc60f2e6433541d387c21e480ff159565/Messages.json' -X POST \
--data-urlencode 'To=whatsapp:'$1 \
--data-urlencode 'From=whatsapp:'$2 \
--data-urlencode 'Body='"$3" \
-u $4:$5
echo "\n"
