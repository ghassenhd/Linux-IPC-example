
echo "Note : run app_a and app_b before running this script."
echo "Note : you can find both of the apps under build directory."
echo "Note : it is recommended to run them in different terminals"
echo "Note : for better visibility and give app_a the option"
echo "Note : -f test/example.json to be able to run this test."
echo "Note : you can do a manual test by editing the json file"

cp example_init.json example.json

echo""
echo "Note : to do a change in the file you can type one of the folowing cmd :"
echo "Note : init, add_ap, del_ap, empty, error_access_points, error_ssid, "
echo "Note : modif_channel, modif_snr, modif_ssid, no_ap."
echo""
echo "Note : anything else will be treated as quit"
echo "Note : to see the change in each cmd just do diff between "
echo "Note : example_init.json and the correspinding file"

read var
while true
do
  case $var in
  
    init)
      cp example_init.json example.json
      ;;

    add_ap)
      cp example_add_ap.json example.json      
      ;;

    del_ap)
      cp example_del_ap.json example.json
      ;;

    empty)
      cp example_empty.json example.json
      ;;

    error_access_points)
      cp example_error_access_points.json example.json
      ;;

    error_ssid)
      cp example_error_ssid.json example.json
      ;;

    modif_channel)
      cp example_modif_channel.json example.json
      ;;

    modif_snr)
      cp example_modif_snr.json example.json
      ;;

    modif_ssid)
      cp example_modif_ssid.json example.json
      ;;

    no_ap)
      cp example_no_ap.json example.json
      ;;

      *)
      break
      ;;
  esac
  read var  
done
