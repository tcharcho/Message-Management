<?php

#SET HIDDEN VARIABLES
$aID = $_POST["authorID"];
$temp = 0;
$first = 0;
$lastUP = 0;
$total = 0;
if(isset($_POST["first"]))
{
	$first = $first + $_POST["first"];
}


if (isset($_POST["radioName"])) 
{
	$str = $_POST["radioName"];
} 
else
{
	$str = $_POST["stream"];
}


#CALL PYTHON PROGRAM TO GET THE NUMBER OF THE LAST UNREAD POST AND THE TOTAL # OF POSTS
$lastCMD = "./getLast.py ". $aID. " -s ". $str; #get last needs an all option
$lastUP = $lastUP + shell_exec($lastCMD);
$totalCMD = "./getTotal.py " . $aID. " -s ". $str; #get total needs an all option
$total = $total + shell_exec($totalCMD);


#SET CURRENT POSITION (AKA. CURRENT POST NUMBER)
if (isset($_POST["currPos"])) 
{
	$temp = $temp + $_POST["currPos"];
	if (isset($_POST["pgUp"]) && $temp > 1 && $temp != 0) 
	{
		$currPos = $_POST["currPos"] - 1;
	} 
	elseif (isset($_POST["pgDown"]) && $temp < $total && $temp != 0) 
	{
	 	$currPos = $_POST["currPos"] + 1;
	}
	elseif(isset($_POST["pgUp"]) && $temp <= 1 && $temp != 0)
	{
		$currPos = $_POST["currPos"];
		echo "You are at the beginning of the stream! No more previous posts to see.";
	}
	elseif (isset($_POST["pgDown"]) && $temp >= $total)
	{
		$currPos = $_POST["currPos"];
		echo "You are at the end of the stream! No more posts to see.";
	}
	elseif (isset($_POST["mRead"]))
	{
		$currPos = $_POST["currPos"];
		echo "All posts have been marked as read.<br>";
		$allreadCMD = "./allRead.py ". $aID. " -s ". $str;
		exec($allreadCMD, $anyErr, $error);
	}
	elseif(isset($_POST["cNew"]))
	{
		$currPos = $lastUP;
	}


	if($total == 0)
        {
                echo "Stream is empty. Check back a little while later or post to stream!<br>";
        }

	if($temp == 0)
	{
		$currPos = 0;
	}
	#temp is my new currPos?
	$temp = 0 + $currPos;
}
else 
{
	if($lastUP > $total)
	{
		$currPos = $total;
		$temp = $temp + $total;
	}
	else
	{
		$currPos = $lastUP;
		$temp = $temp + $lastUP;
	}
	
	if($total == 0)
        {
                echo "Stream is empty. Check back a little while later or post to stream!<br>";
        }
	$first = $first + $currPos;
}

#MARK CURRENT PAGE AS READ
if ($temp == $lastUP && $total != 0)
{
	$updateCMD = "./markRead.py ". $aID. " -s ". $str;
        exec($updateCMD, $iferr, $rv);
        #if there was an error, print it here
}


if($currPos < 0)
{
	echo "Oops! Can't go back any further. You've already read those<br>";
	$currPos = $currPos+1;
	
	$getpostCMD = "./getPost.py ". $aID. " -s ". $str. " ". $currPos;
        exec($getpostCMD, $nextPost, $retVal);
}
else 
{
	$getpostCMD = "./getPost.py ". $aID. " -s ". $str. " ". $currPos;
	exec($getpostCMD, $nextPost, $retVal);
}




$command = "./config view.wpml";
exec($command, $result, $val);

foreach ($result as $line) 
{ 
	if (strcmp($line, "</form>") == 0) 
	{
		echo "<input type=\"submit\" name=\"pgUp\" value=\"<-- Page Up\">";
		echo "<input type=\"submit\" name=\"pgDown\" value=\"Page Down -->\">";
		echo "<br>";
		echo "<input type=\"submit\" name=\"mRead\" value=\"Mark all Read\">";
		echo "<input type=\"submit\" name=\"cNew\" value=\"Check for New Posts\">";

		echo "<input type=\"hidden\" name=\"currPos\" value=$currPos>";
		echo "<input type=\"hidden\" name=\"authorID\" value=$aID>";
	        echo "<input type=\"hidden\" name=\"stream\" value=$str>";
		echo "<input type=\"hidden\" name=\"first\" value=$first>";
	}

	if (strcmp($line, "</textarea>") == 0 ) 
	{
		#echo the output of getPost.py, which will spit out the next/previous post to post. 
		foreach ($nextPost as $postLine) 
		{
			echo "$postLine\n";
		}
		
	} 
	
	echo "$line";

	if (strcmp($line, "<HR>") == 0) 
	{
		echo "Author ", $aID, " is viewing the ", $str, " stream <br>";
	}

	if (strcmp($line, "</form>") == 0) 
	{
		echo "<form method=\"post\" action=\"setInfo.php\">";
                echo "<input type=\"submit\" value=\"Select New Stream\">";
		echo "<input type=\"hidden\" name=\"authorID\" value=$aID>";
                echo "</form>";
		echo "<form method=\"post\" action=\"addRemove.php\">";
		echo "<input type=\"submit\" value=\"Add/Remove Author\">";
		echo "</form>";
		echo "<form method=\"post\" action=\"post.php\">";
                echo "<input type=\"submit\" value=\"Post\">";
		echo "<input type=\"hidden\" name=\"authorID\" value=$aID>";
                echo "<input type=\"hidden\" name=\"stream\" value=$str>";
                echo "</form>";
		echo "<form method=\"post\" action=\"index.php\">";
		echo "<input type=\"submit\" value=\"Home\">";
		echo "</form>";

	}

	
}

?>
