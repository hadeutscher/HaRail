HaRail
======

Public transport fastest-route finder for Israel Railways

Compile against VC 12.0 or upper (VS 2012\2013), with boost libs (mine is 1.57). Edit project include and lib directories to point to your boost installation.

The program expects to find a GTFS database; at C:\irw_gtfs for debug and at .\irw_gtfs for release. You can get IRW_GTFS at ftp://gtfs.mot.gov.il/irw_gtfs.zip (see http://he.mot.gov.il/index.php?option=com_content&view=article&id=2244:pub-trn-gtfs&catid=167:pub-trn-dev-info&Itemid=304 for more info). IRW's GTFS database is valid for half a year since its download date.