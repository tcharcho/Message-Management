<?php

if ( isset ( $_POST["add"] ) )
{
    $com =  "./addauthor ". $_POST["authorID"]. " -s ". $_POST["streams"];
    exec($com, $result, $retVal);

    foreach($result as $line)
    {
        echo "$line". "<br>";
    }
}
elseif ( isset ( $_POST["remove"] ) )
{
    $com =  "./addauthor -r ". $_POST["authorID"]. " -s ". $_POST["streams"];
    exec($com, $result, $retVal);
    foreach($result as $line)
    {
        echo "$line". "<br>";
    }

}

exec("./config addRemove.wpml", $html_code, $return_val);
foreach ($html_code as $key) 
{
	echo "$key";
}

?>

