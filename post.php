<?php
if (isset($_POST["authorID"]) && isset($_POST["stream"]))
{
	$aID = $_POST["authorID"];
	$str = $_POST["stream"];

	if(isset($_POST["thePost"]))
    	{
        	$com = "./post ". $_POST["authorID"]. " -s ". $_POST["stream"];

        	$fh = fopen("messages/post.txt", "w");
        	fwrite($fh, $_POST["thePost"], strlen($_POST["thePost"]));
        	fclose($fh);


        	exec($com, $res, $val);
        	foreach($res as $line)
        	{
            		echo "$line<br>";
        	}

        	#unlink("messages/post.txt");
        }



	$com = "./config post.wpml";
	exec($com, $result, $val);
	foreach($result as $line)
	{
	    if(strcmp($line, "</form>") == 0)
	    {
		echo "<input type=\"hidden\" name=\"authorID\" value=$aID>";
	        echo "<input type=\"hidden\" name=\"stream\" value=$str>";
	    }
	    echo $line;
	}

}
else
{
    header("Location: getInfo.php");

}
?>
