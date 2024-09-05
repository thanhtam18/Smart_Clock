#ifndef _TDEV_WEBINFO_H
#define _TDEV_WEBINFO_H
#include <Arduino.h>

String form = R"(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>TDev</title>
<style>
.login-page {
width: 360px;
padding: 8% 0 0;
margin: auto;
}
.form {
position: relative;
z-index: 1;
background: #FFFFFF;
max-width: 360px;
margin: 0 auto 100px;
padding: 45px;
text-align: center;
box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);
}
.form input {
font-family: "Roboto", sans-serif;
outline: 0;
background: #f2f2f2;
width: 100%;
border: 0;
margin: 0 0 15px;
padding: 15px;
box-sizing: border-box;
font-size: 14px;
}
.form button {
font-family: "Roboto", sans-serif;
text-transform: uppercase;
outline: 0;
background: #4CAF50;
width: 100%;
border: 0;
padding: 15px;
color: #FFFFFF;
font-size: 14px;
-webkit-transition: all 0.3 ease;
transition: all 0.3 ease;
cursor: pointer;
}
.form button:hover,
.form button:active,
.form button:focus {
background: #43A047;
}
body {
margin: 0;
padding: 0;
font-size: 1.25rem;
background: #FAFAFA;
display: block;
align-items: center;
text-align: center;
}
</style>
</head>
<div class="login-page">
<div class="form">
<form class="login-form" method="GET" action="/setgps">
<h2>Update GPS</h2>
)";



String end_form = R"(<button type="submit">Apply</button>
</form>
</div>
</div>
</body>
</html>)";

String set_complete = R"(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>TDEV</title>
<style>
body {
margin: 0;
padding: 0;
font-size: 1.25rem;
background: #FAFAFA;
display: block;
align-items: center;
text-align: center;
}
button {
font-family: "Roboto", sans-serif;
text-transform: uppercase;
outline: 0;
background: #4CAF50;
width: 300px;
border: 0;
padding: 15px;
color: #FFFFFF;
font-size: 14px;
-webkit-transition: all 0.3 ease;
transition: all 0.3 ease;
cursor: pointer;
}
</style>
</head>
<body>
<H1>Setting result</H1>
<p>Setting Successful</p>
<a href="/restart"><button>Click here to restart</button></a>
</body>
</html>)";

#endif