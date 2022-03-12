<!doctype html>
<html lang="fr">
	<head>
		<meta charset="utf-8">
		<title>D E L E T E</title>
		<link rel="shortcut icon" href="/favicon/favicon_2.ico" type="image/x-icon">
    	<link rel="icon" href="/favicon/favicon_2.ico" type="image/x-icon">
		<link rel="stylesheet" href="/style.css">
		<link rel="stylesheet" href="/get_style.css">
        <link rel="stylesheet" href="/thunder.css">

	</head>
	<body>
        <table class="center">
                <tr>
                    <th>
                        <form action="../index.html">
                            <button type="submit" id="post_form" class="btn">
                                <pre class="btn-pre">H O M E   P A G E</pre>
                            </button>
                        </form>
                    </th>
                    
                </tr>

        </table>
        <div class="content">
            <div class="form-div content-div">
                <div>
                </div> 
                <form action="delete.php" method="GET">
                <pre> D E L E T E </pre>
                <input class="input" name="dfile" id="name" required>
                <br>
                <button type="submit" id="get_form" class="formbtn"> 
                    <pre class="btn-pre">S U B M I T</pre>
                </button>
                <?php
                    if (isset($_GET['dfile'])) {
                        shell_exec('../script/delete.sh '.$_GET['dfile'].'');
                        // header("Location: /page/delete.php");
                    }
                ?>
            </form>
        </div>
            <div class="result content-div">
            <list>
            <pre> F I L E S : </pre>
            <?php
                echo "\n";
                foreach (glob("../upload/*") as $filename) {
                    echo "&nbsp" .  "&nbsp" . substr($filename, 10) . "<br>";
                }
                ?>
            </list>
            </div>
        <div id="container">
            <h1>削除するには</h1>
            <h1>削除するには</h1>
            <h1>削除するには</h1>
            <h1>削除するには</h1>
            <h1>削除するには</h1>
        </div>


	</body>
</html>