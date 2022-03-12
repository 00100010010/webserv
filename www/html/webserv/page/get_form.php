<!doctype html>
<html lang="fr">
	<head>
		<meta charset="utf-8">
		<title>Get form</title>
		<link rel="shortcut icon" href="/favicon/favicon_2.ico" type="image/x-icon">
    	<link rel="icon" href="/favicon/favicon_2.ico" type="image/x-icon">
		<link rel="stylesheet" href="/style.css">
		<link rel="stylesheet" href="/get_style.css">
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
		<script src="../script/get_info.js"></script>
	</head>
	<body>
		<table class="center">
			<tr>
				<th>
					<form action="../index.html">
						<button type="submit" id="get_form" class="btn">
							<pre class="btn-pre">H O M E   P A G E</pre>
						</button>
					</form>
				</th>
				
			</tr>

		</table>
		<div class="content">
			<div class="form-div content-div">
				<form action="get_form.php" method="GET">
					<pre> Register Form </pre>
					<label>Last Name : </label></br>
					<input class="input" name="last_name" id="name" required>
					<br>
					<label>First Name : </label></br>
					<input class="input" name="first_name" id="name" required>
					<br>
					<label>Email : </label></br>
					<input class="input" name="email" id="name" required>
					<br>
					<label>Login : </label></br>
					<input class="input" name="login" id="name" required>
					<br>
					<button type="submit" id="get_form" class="formbtn">
						<pre class="btn-pre">S U B M I T</pre>
					</button>
				</form>
			</div>
			<div class="result content-div">
				<pre> Last form result : </pre>
				<?php
					if(isset($_GET['last_name'])) {
						echo "<pre class=\"value\">";
						echo "Last Name : ";
						echo $_GET['last_name'];
						echo "</pre>";
					}
					if(isset($_GET['first_name'])) {
						echo "<pre class=\"value\">";
						echo "First Name : ";
						echo $_GET['first_name'];
						echo "</pre>";
					}
					if(isset($_GET['email'])) {
						echo "<pre class=\"value\">";
						echo "Email : ";
						echo $_GET['email'];
						echo "</pre>";
					}
					if(isset($_GET['login'])) {
						echo "<pre class=\"value\">";
						echo "Login : ";
						echo $_GET['login'];
						echo "</pre>";
					}
				?>
			</div>
		</div>

		<div class="info" id="info" onclick="content();" style="height: 30px;">
			<div class="xdiv" id="xdiv"> ? </div>
			<div class="info_content" id="info_content">
				
				<?php
					$file = file("../../../../headers.log");
					$line = $file[count($file)- 1];
					print_r(str_replace('	', '<br>', $line));
				?>

			</div>
		</div>

		<canvas id="scene" width="1000" height="500"></canvas>
		<script src="../script/glob.js"></script>
		<div class="credit">
			<pre> <a href="https://codepen.io/Mamboleoo">by Louis Hoebregts</a></pre>
		</div>

	</body>
</html>