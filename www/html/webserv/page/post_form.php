<!doctype html>
<html lang="fr">
	<head>
		<meta charset="utf-8">
		<title>Post form</title>
		<link rel="shortcut icon" href="/favicon/favicon_2.ico" type="image/x-icon">
    	<link rel="icon" href="/favicon/favicon_2.ico" type="image/x-icon">
		<link rel="stylesheet" href="/style.css">
		<link rel="stylesheet" href="/get_style.css">
		<link rel="stylesheet" href="/vortex.css">
		<script src="../script/get_info.js"></script>
	</head>
	<body>
		<div id="blackhole">
			<div class="centerHover"><span class="hvr-shrink"></span></div>
		</div>
		<script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.1.0/jquery.min.js'></script>
		<script src="../script/vortex.js"></script>
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
				<form action="post_form.php" method="POST">
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
					if(isset($_POST['last_name'])) {
						echo "<pre class=\"value\">";
						echo "Last Name : ";
						echo $_POST['last_name'];
						echo "</pre>";
					}
					if(isset($_POST['first_name'])) {
						echo "<pre class=\"value\">";
						echo "First Name : ";
						echo $_POST['first_name'];
						echo "</pre>";
					}
					if(isset($_POST['email'])) {
						echo "<pre class=\"value\">";
						echo "Email : ";
						echo $_POST['email'];
						echo "</pre>";
					}
					if(isset($_POST['login'])) {
						echo "<pre class=\"value\">";
						echo "Login : ";
						echo $_POST['login'];
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
		<div class="credit">
			<pre> <a href="https://codepen.io/normacttekcas">by normacttekcas</a></pre>
		</div>
		
	</body>

</html>