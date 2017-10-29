<?php
$com2 =  "./config getInfo.wpml";
exec($com2, $rt, $retVal);
foreach($rt as $ln)
{
    echo "$ln";
}
?>
