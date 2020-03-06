const char* MAIN_page = R"HTML(<!DOCTYPE html>
<html>
<head>
<title>XMAS</title>
<style>
body { background-color: black; color: white; font-family: verdana; font-size: 24pt; }
input { font-size: 24pt; }
</style>
</head>
<body>
<h1>Merry XMAS &#128522;</h1>
<p>Please enter the text message to display and push the button.</p>
<form action="/" method="post">
Animation delay: <input type="number" name="animDelay" value="%animDelay%"><br>
Text font: <input type="number" name="textFont" value="%textFont%"><br>
Text size: <input type="number" name="textSize" value="%textSize%"><br>
Message: <input type="text" name="message" value="%message%"><br>
<input type="submit">
</form>
</body>
</html>
)HTML";