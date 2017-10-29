<?php
$comm = "./db -create";
shell_exec($comm);
echo <<<_TAG
    <html>
    <body>
        <h3>Welcome!</h3> <hr>

        <form action="setInfo.php" method="post">
        Please enter your author ID: <input type="text" name="authorID" placeholder="Author ID"><br>
        <input type="submit" name="view", value="View">
        </form>
	<hr>
        
        <form action="addRemove.php" method="post" style="display: inline;">
        <input type="submit" name="addRemoveAuthor", value="Add/Remove Author">
        </form>

        <form action="getInfo.php" method="post" style="display: inline;">
        <input type="submit" name="toPost", value="Post">
        </form>

    </body>
    </html>
_TAG;


?>

