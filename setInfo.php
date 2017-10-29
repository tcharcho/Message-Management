<?php
$command = "./getStreams.py ". $_POST["authorID"];
exec($command, $ret, $val);
$aID = $_POST["authorID"];
if ($val == 0) 
{
	foreach($ret as $line)
	{
	    echo "$line<br>";
	}
	echo "<html>";
	echo "<form method=\"post\" action=\"index.php\" style=\"display: inline;\">";
	echo "<input type=\"submit\" value=\"Home\">";
	echo "</form>";
	echo "<form method=\"post\" action=\"addRemove.php\" style=\"display: inline;\">";
	echo "<input type=\"submit\" value=\"Add/Remove Author\">";
	echo "</form>";
	echo "</html>";
}
else
{
    $com = "./config setInfo.wpml";
    exec($com, $output, $retVal);
    foreach($output as $code)
    {
	if(strcmp($code,"<input type=\"radio\" name=\"radioName\" value=\"all\" checked>all<br>") == 0)
	{
		foreach ($ret as $line) 
		{
			echo "<input type=\"radio\" name=\"radioName\" value=\"$line\">$line<br>";
		}
	}

	if(strcmp($code, "</form>") == 0)
	{
	    echo "<input type=\"hidden\" name=\"authorID\" value=$aID>";
	}
	echo "$code";
    }

}

?>
