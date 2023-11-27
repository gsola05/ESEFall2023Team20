const char *HTML_CONTENT = R""""(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="description" content = "ESE Team 20 has made WindowPal - An automatical window control system">
        <meta name="viewport" content="width=device-width, intial-scale=1.0">
        <meta http-equiv="refresh" content="3">
        <title>ESE Team 20 - WindowPal</title>

        <style>
            .split {
                height: 100%;
                width: 50%;
                position: fixed;
                z-index: 1;
                top: 0;
                overflow-x: hidden;
                padding-top: 20px;
            }

            .left {
                left: 0;
            }

            .right {
                right: 0;
            }

            .centered {
                position: absolute;
                top: 50%;
                left: 50%;
                transform: translate(-50%, -50%);
                text-align: center;
            }
        </style>
    </head>
    <body>
        <p style="text-align: center; font-size: 2.75em;">You Window is OPEN_HOLDER</p>

        <div class="split left">
            <div class="centered">
                <p style="font-size: 1.25em;">The current temperature is TEMP_HOLDER °C</p>
            </div>
        </div>

        <div class="split right">
            <div class="centered">
                <p style="font-size: 1.25em;">The current humidity is HUM_HOLDER %</p>
            </div>
        </div>
    </body>
</html>
)"""";