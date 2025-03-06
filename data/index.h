const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
 <style>
       html {

            font-family: Arial;
            margin: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            text-align: center;
            color: #FFFFFF;
        }

        h2 {
            font-size: 3.0rem;
            color: #d6d6d6;

        }

        h5,
        h6 {
            margin: 8px 0;
            padding: 0;
        }

        p {
            font-size: 3.0rem;
        }

        body {
            font-family: Tahoma, sans-serif;
            background-color: #101b33;
            margin: 0;
        }

        .switch-button {
            display: flex;
            align-items: center;
            justify-content: center;
            margin: 0;
            height: 40px;
        }

        .switch-button .switch-outer {
            background: #252532;
            height: 40px;
            width: 80px;
            border-radius: 165px;
            box-shadow: inset 0px 5px 10px 0px #16151c, 0px 3px 6px -2px #403f4e;
            border: 1px solid #32303e;
            padding: 6px;
            box-sizing: border-box;
            cursor: pointer;
        }



        .switch-button .switch-outer input {
            opacity: 0;
            appearance: none;
            position: absolute;
        }

        .switch-button .switch-outer .button-toggle {
            height: 28px;
            width: 28px;

            background: linear-gradient(#3b3a4e, #272733);
            border-radius: 100px;
            box-shadow: inset 0px 5px 4px 0px #424151, 0px 4px 15px 0px #0f0e17;
            position: relative;
            z-index: 1;
            transition: left 0.3s ease-in;
            left: 0;
        }

        .switch-button .switch-outer input:checked+.button .button-toggle {
            left: 36px;
        }

        .switch-button .switch-outer input:checked+.button .button-indicator {
            animation: indicator 1s forwards;
        }

        .switch-button .switch-outer .button {
            width: 65px;
            height: 28px;
            display: -webkit-box;
            display: -ms-flexbox;
            display: flex;
            position: relative;
            justify-content: space-between;
        }

        .switch-button .switch-outer .button-indicator {
            height: 15px;
            width: 15px;
            top: 14px;
            transform: translateY(-8px);
            border-radius: 50px;
            border: 3px solid #ef565f;
            box-sizing: border-box;
            right: 10px;
            position: relative;

        }



        @keyframes indicator {

            from {
                opacity: 1;
            }

            to {
                opacity: 1;
                border: 3px solid #60d480;
                left: -45px;
            }
        }

        .btn-box {
            max-width: 450px;
            margin: 16px;
            box-sizing: border-box;
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
            position: relative;


        }

        .btn {
            position: relative;
            margin: 16px;
            height: 100px;
            width: 300px;
            row-gap: 10px;
            background-color: #37374d;
            border-radius: 12px;
            display: flex;
            box-shadow: rgba(0, 0, 0, 0.658) 0px 3px 6px;
            flex-direction: column;
            justify-content: space-between;
            align-items: start;
            padding: 20px;
            transition: margin 500ms ease-in-out;
            margin: 0px 0px 20px 0px;
            clip-path: view-box;
        }

        .bottom {
            position: absolute;
            bottom: 0;
            left: -10px;
            z-index: -10;
            height: 60px;
            width: 350px;
            background-color: #17173d;
            opacity: 0.5;
        }

        .btn_index {
            position: absolute;
            font-size: 160px;
            z-index: 0;
            opacity: 0.1;
            color: #111525;
            left: -12px;
            top: -30px;
            font-weight: 900;
        }

        .btn.show {
            margin: 0px 0px 220px 0px;
        }

        .editor-1.show {
            transform: translate(0px, 135px);
        }
 .antenna{
            display: flex;
            align-items: end;
            padding-bottom: 4px;
            margin-left: 14px;
        }
        .antenna>li{

            margin: 0px 2px;
            box-sizing: border-box;
            padding: 0;

        }
        .editor-1 {
            box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 18px;
            background-color: #e5e5e5;
            color: #fca311;
            position: absolute;
            top: 50vh;
            left: 50vw;
            transform: translate(-35vw, -35vh);
            height: 430px;
            width: 310px;
            border-radius: 18px;
            box-sizing: border-box;
            padding: 0 16px;
            border: #566063 solid 0.2px;
            transition: transform 500ms ease-in-out;
            flex-direction: column;
            margin: auto;
            text-align: center;

        }

        .s-dialog {
            background-color: #000000be;
            z-index: 1000;
            height: 100vh;
            width: 100vw;
            margin: auto 0;
        }

        .editor-1 span {
            color: #d6d6d6;
            padding: 0 0px;
        }

        .editor-1 h4 {
            color: #14213d;
        }

        .timer-title {
            width: 30px;
            font-size: 12px;
            color: #b19ed6;
            font-weight: 600;
            padding: 0;
        }

        .empty {
            width: 54px;
            height: 20px;
        }

        .timer_div {
            cursor: pointer;
            font-size: 24px;
        }

        .row {
            display: flex;
            width: 280px;
            margin: 0 auto;
            justify-content: space-between;
            align-items: end;

        }

        .title_wrapper {
            position: relative;
            display: flex;
            flex-direction: row;

        }

        .title {
            font-size: 1.3rem;
            font-weight: 600;
            margin: 0px 0px 10px 0px;
            color: #cdc1d6;
            background-color: #37374d;
            width: 190px;
            border: none;
            box-sizing: border-box;
            background-color: transparent;
            outline: none;


        }

        .title[type=text]:focus {
            border: none;
        }

        .title-btn {
            position: absolute;
            top: 28px;
            right: 140px;
            border: none;
            z-index: 200;
            width: 10px;
            color: #0fa735;
        }

        .title_cancel {
            position: absolute;
            top: 28px;
            left: 16px;
            border: none;
            z-index: 200;
            width: 10px;
            color: #a70f5b;
        }

        .sch {
            color: #488067;
            align-items: end;
            padding: 0 10px;
            font-size: 12px;

        }

        .sch>span {
            padding: 0 10px;
            font-size: 12px;
            z-index: 100;
            cursor: pointer;
        }


        .start {
            display: flex;
            flex-direction: column;
            width: 50px;
            align-items: start;
        }

        .timebox {
            display: flex;
            flex-direction: column;
            padding: 0 4px;

        }

        .timebox span {
            text-align: left;

        }

        .row-times {
            display: flex;
            width: auto;
            justify-content: center;
            align-items: center;
            cursor: pointer;

        }


        .row-times span {
            color: #000;
            font-weight: 600;
            text-align: left;
        }

        .row-time {
            display: flex;
            height: 60px;
            justify-content: start;
            align-items: center;
            background-color: #ffffff;
            border-radius: 18px;
            margin: 12px 0;
            text-align: left;

        }



        .row-labels {
            text-align: left;
            display: flex;
            justify-content: space-around;
            align-items: start;
            padding: 10px 0;
        }

        .buttons {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-top: 20px;
        }

        button {
            height: 40px;
            width: 130px;
            min-width: none;
            font-size: 16px;
            font-weight: 600;
            border-radius: 10px;
            border: none;
            color: #222222;
            background-color: #c2c2c2;
        }

        button.active {
            background-color: #91cca0;
        }
        #gsm {
            color: #9AACB0;
            display: flex;
            align-items: end;
            font-size: 14px;
            margin-left: 8px;
        }
        .radio-btn {
            margin: 0 auto;
            border: none;
            height: 18px;
            width: 18px;
            margin-right: 14px;
            outline: none;
        }
        .sta-dialog {
            background-color: #e5e5e5;
            height: 150px;
            width: 280px;
            margin: 0 auto;
            text-align: center;
            border: none;
            border-radius: 16px;
            box-shadow: #2c2c2c 0px 4px 10px;
            color: #007896;
            padding: 16px 0;
            position: absolute;
            top: 50vh;
            left: 50vw;
            transform: translate(-30vw, -20vh);
            border-radius: 18px;
            box-sizing: border-box;
            padding: 0 16px;
            border: #566063 solid 0.2px;
            transition: transform 500ms ease-in-out;
            display: flex;
            flex-direction: column;
            margin: auto;
            text-align: center;
            justify-content: space-around;
        }

        ul {
            list-style: none;
            position: relative;
            cursor: pointer;
            padding: 2px 0px;
        }
        li {
            display: inline;
            padding: 0 4px;
            position: relative;
        }
        .inner-title {
            font-size: 12px;
            color: #16151c;
            text-align: left;
            margin: 0 auto;
            padding: 0 auto;
            font-weight: 600;
        }
        .timer_alert {
            box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 18px;
            background-color: #e5e5e5;
            color: #fca311;
            position: absolute;
            top: 50vh;
            left: 50vw;
            transform: translate(-30vw, -20vh);
            height: 210px;
            width: 310px;
            border-radius: 18px;
            box-sizing: border-box;
            padding: 0 16px;
            border: #566063 solid 0.2px;
            transition: transform 500ms ease-in-out;
            display: flex;
            flex-direction: column;
            margin: auto;
            text-align: center;
            justify-content: space-around;
        }

        .timer_alert>span {
            color: #000;
        }
    </style>
</head>

<body>
    <h4>Smart Switch</h4>
    <section class="row-times">

        <h5>GSM Signal Quality : <h6 id="gsm"></h6>
        </h5>
    </section>
    <section class="row-times">
        <h5 id="sensor_1"> SENSOR : <h6 id="sensor1"></h6>
        </h5>

    </section>


    <div class="btn-box" id="btn-box">
    </div>

    <dialog class="s-dialog" id="editor-1">
        <div class="editor-1">
            <h4>Timer Setting</h4>
            <!-- weekdays -->
            <div class="inner-title">Week Days:</div>
            <ul class="days" id="list"></ul>

            <!-- times -->
            <div class="inner-title">Timer Type:</div>

            <div class="row-time">
                <input class="radio-btn" type="radio" id="start-stop" name="timer-type" value="start-stop">

                <div class="timebox">
                    <label>From</label>
                    <section class="row-times">
                        <div class="start">
                            <select name="start-hour" id="start-hour"></select>
                        </div>
                        <span>:</span>
                        <div class="start">
                            <select name="start-min" id="start-min"></select>
                        </div>
                    </section>
                </div>

                <div class="timebox">
                    <label>To</label>
                    <section class="row-times">
                        <div class="start">
                            <select name="end-hour" id="end-hour"></select>
                        </div>
                        <span>:</span>

                        <div class="start">
                            <select name="end-min" id="end-min"></select>
                        </div>
                    </section>
                </div>
                </>

            </div>

            <div class="row-time">
                <input class="radio-btn" type="radio" id="interval" name="timer-type" value="interval">

                <div class="timebox">
                    <label>Per Hours</label>
                    <section class="row-times">
                        <div class="start">
                            <select name="per_h" id="per_h"></select>
                        </div>
                        <div class="empty"></div>

                    </section>
                </div>

                <div class="timebox">
                    <label>To</label>
                    <section class="row-times">
                        <div class="start">
                            <select name="interval_h" id="interval_h"></select>
                        </div>
                        <span>:</span>

                        <div class="start">
                            <select name="interval_m" id="interval_m"></select>
                        </div>
                    </section>
                </div>
                </>

            </div>


            <div class="row-time">
                <input class="radio-btn" type="radio" id="off" name="timer-type" value="off">
                <label>Disable</label>
                <div class="empty"></div>
                <div class="empty"></div>

            </div>

            <!-- buttons -->

            <div class="buttons">
                <button class="active" type="button" onclick=saveTimer(this)>
                    Save
                </button>
                <button type="button" onclick=closeMyModal(0)>
                    Cancel
                </button>
            </div>
        </div>
    </dialog>



    <dialog class="s-dialog" id="s-dialog" value="error">
        <div class="sta-dialog" id="sta-dialog">
           

        </div>
    </dialog>

    <dialog class="s-dialog" id="timer_alert">
        <div class="timer_alert">
            <h4>
                Disable Timer
            </h4>
            <span>
                Are you sure to disable timer?
            </span>
            <div class="buttons">
                <button class="active" type="button" onclick=setRelay(this)>
                    Yes
                </button>
                <button type="button" onclick='closeMyModal("timer_alert")'>
                    Cancel
                </button>
            </div>
        </div>
    </dialog>


    <script>
        var PlaceHolder = '%BUTTONPLACEHOLDER%';
        var relays;
        var jsonData = JSON.parse(PlaceHolder);
        let selectedId;

        const weekDays = ["Sa", "Su", "Mo", "Tue", "Wed", "Thu", "Fri"];
        let daysInt = [];
        let s_h;
        let s_m;
        let e_h;
        let e_m;
        let per_h;
        let interval_h;
        let interval_m;

        window.onload = onLoaded;

       function showDialog(text, type) {
            const dialog = document.getElementById('s-dialog');
            const dialog_container = document.getElementById('sta-dialog');
            const dialog_icon = document.createElement('div');
            dialog_container.innerHTML = "";

            dialog_icon.innerHTML = type === "error" ? "&#x2715" :"&#x2716" ;
            dialog_icon.style.color = type === "error" ? "red" :`#616179`;
            dialog_icon.style.fontSize =  "28px";

            var newPara = document.createElement('h3');
            newPara.innerText = text ?? 'Success';
            newPara.style.color =  type === "error" ?  "red"  : `#616179`;
            dialog_container.appendChild(dialog_icon);
            dialog_container.appendChild(newPara);
            //dialog.style.backgroundColor = col;
            //circle.style.color = col;

            dialog.show();
            setTimeout(() => dialog.close(), 1500);
        }

        function closeDialog() {
            const dialog = document.getElementById('s-dialog');
            dialog.close();
        }

        function setRelay() {
            const dialog = document.getElementById("timer_alert");
            const checkbox = dialog.value.item;
            const index = dialog.value.index;
            const state = dialog.value.state === 1;
            relays[index].state = state ? 1 : 0;
            var xhr = new XMLHttpRequest();
            if (state) { xhr.open("GET", "/update?output=" + index + "&state=0", true); }
            else { xhr.open("GET", "/update?output=" + index + "&state=1", true); }
            xhr.send();
            xhr.onreadystatechange = function () {
                if (this.readyState == 2 && this.status == 200) {
                    checkbox.checked = !state;
                    console.log(dialog.value);
                } else {
                    showDialog("Connection Error!","error");
                    console.log(this.status);
                }

            };
            closeMyModal("timer_alert");

        }
         function modelJson(data) {
            console.log(jsonData);
            // Convert JSON data to an array of Relay objects

            relays = Object.keys(data)
                .filter(key => !isNaN(key)) // Filter out non-numeric keys
                .map(key => new Relay(data[key].name, data[key].timer, data[key].state));

            const switchBox = document.getElementById("btn-box");


            //create switchs
            relays.forEach((deviceName, index) => {
                const switchElement = createSwitch(index, relays[index].name, relays[index].state, relays[index].timer);
                switchBox.appendChild(switchElement);
            });

            //update network value
            document.getElementById("gsm").innerText = data["network"];


        }
        function convertStringToHexUnicode(originalString) {
            let formattedString = Array.from(originalString).map(char => {
                let codePoint = char.codePointAt(0);
                return codePoint.toString(16).padStart(4, '0');
            }).join('');
            return formattedString;
        }
        let persianUnicodes = "^[\u0600-\u06FF\uFB8A\u067E\u0686\u06AF\u200C\u200F]+$";
        let regex = new RegExp(persianUnicodes);
        function createSwitch(index, deviceName, deviceState, deviceTimer) {
            // add divice label
            const title_wrapper = document.createElement("div");
            const label_relay = document.createElement("input");
            const title_btn = document.createElement("div");
            const title_cancel = document.createElement("div");
            label_relay.classList.add("title");
            label_relay.id = "title" + index;
            title_btn.hidden = true;
            title_cancel.hidden = true;
            title_btn.classList.add("title-btn");
            title_cancel.classList.add("title_cancel");
            label_relay.addEventListener("input", (value) => {
                if (label_relay !== value.srcElement.value) {
                    title_btn.hidden = false;
                    title_cancel.hidden = false;
                } else {
                    title_btn.hidden = true;
                    title_cancel.hidden = true;
                }
                console.log(value.srcElement.value);
            });
            label_relay.addEventListener("focus", (value) => {
                if (deviceName[0] == "0") {
                    label_relay.value = convertHexUnicodeToString(deviceName);
                } else {

                    label_relay.value = deviceName[0].toUpperCase() + deviceName.slice(1);
                }
                title_btn.hidden = true;

                console.log(deviceName);
            });
            title_btn.addEventListener("click", (value => {

                var xhr = new XMLHttpRequest();
                let isMatch = regex.test(label_relay.value);
                let label;
                if (isMatch) {
                    label = convertStringToHexUnicode(label_relay.value);
                } else {
                    label = label_relay.value;
                }
                let params = `${index}>${label}`;

                xhr.open("POST", "/label", true);
                xhr.onreadystatechange = function () {
                    if (this.readyState == 2 && this.status == 200) {
                        let title = document.getElementById("title" + index);
                        label_relay.value = title.value;
                        console.log(value);
                    } else {

                        title_btn.hidden = true;
                        title_cancel.hidden = true;
                        if (deviceName[0] == "0") {
                            label_relay.value = convertHexUnicodeToString(deviceName);
                        } else {

                            label_relay.value = deviceName[0].toUpperCase() + deviceName.slice(1);
                        }


                        console.log(this.status);
                    }
                };
                xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');

                xhr.send(params);
                console.log(label_relay.value);

                // Here you can implement your logic to send the updated state to a server or perform other actions.
            }));
            title_cancel.addEventListener("click", (value => {
                if (deviceName[0] == "0") {
                    label_relay.value = convertHexUnicodeToString(deviceName);
                } else {
                    label_relay.value = deviceName[0].toUpperCase() + deviceName.slice(1);
                }
                title_btn.hidden = true;
                title_cancel.hidden = true;
                console.log(label_relay.value);

                // Here you can implement your logic to send the updated state to a server or perform other actions.
            }));

            title_btn.innerText = "save";
            title_cancel.innerText = "x";
            title_wrapper.classList.add("title-wrapper");
            if (deviceName[0] == "0") {
                label_relay.value = convertHexUnicodeToString(deviceName);
            } else {

                label_relay.value = deviceName[0].toUpperCase() + deviceName.slice(1);
            }

            title_wrapper.appendChild(label_relay);
            title_wrapper.appendChild(title_btn);
            title_wrapper.appendChild(title_cancel);


            //const switchContainer = document.createElement("label");
            // switchContainer.classList.add("switch");

            //add switch checkbox

            const label = document.createElement("label");
            label.classList.add('switch-button');
            label.for = "switch";
            const switch_outer = document.createElement("div");
            switch_outer.classList.add('switch-outer');
            label.appendChild(switch_outer);
            const checkbox = document.createElement("input");
            checkbox.id = "switch";
            checkbox.type = "checkbox";
            checkbox.checked = deviceState === 1;
            checkbox.addEventListener("click", () => {
                const prevState = deviceState === 1;
                if (deviceTimer === "") {
                    relays[index].state = checkbox.checked ? 1 : 0;
                    var xhr = new XMLHttpRequest();
                    if (checkbox.checked) { xhr.open("GET", "/update?output=" + index + "&state=0", true); }
                    else { xhr.open("GET", "/update?output=" + index + "&state=1", true); }
                    xhr.send();
                } else {
                    checkbox.checked = prevState;
                    const dialog = document.getElementById("timer_alert");
                    dialog.value = {
                        item: checkbox,
                        index: index,
                        state: deviceState,
                    };
                    dialog.showModal();
                }


                // Here you can implement your logic to send the updated state to a server or perform other actions.
            });

            switch_outer.appendChild(checkbox);
            const button = document.createElement('div');
            button.classList.add('button');
            switch_outer.appendChild(button);
            const span1 = document.createElement('span');
            span1.classList.add('button-toggle');
            const span2 = document.createElement('span');
            span2.classList.add('button-indicator');

            button.appendChild(span1);
            button.appendChild(span2);

            //const slider = document.createElement("span");
            //slider.classList.add("slider");

            //switchContainer.appendChild(checkbox);
            //switchContainer.appendChild(slider);

            const timerTitle = document.createElement("span");
            timerTitle.classList.add("timer-title");
            timerTitle.textContent = "Active Timer";
            timerTitle.value = deviceTimer;
            //timerTitle.hidden = deviceTimer === "";

            const row1 = document.createElement("div");
            row1.classList.add("row");

            const row2 = document.createElement("div");
            row2.classList.add("row");

            const editPageSection = document.createElement("section");
            editPageSection.id = index;
            editPageSection.classList.add("sch");


            if (deviceTimer === "") {

                const addTimer = document.createElement("span");
                addTimer.textContent = "+ Set Timer";
                addTimer.style.cursor = "pointer";
                addTimer.style.color = "#57cc99";
                editPageSection.appendChild(addTimer);
                //editPageSection.innerHTML = "<div><span style=\"cursur:pointer; cursur:pointer;\">+ Set Timer<span></div>";
                //editPageSection.style.color = "#57cc99";
                addTimer.onclick = function () {
                    openEditPage(index, this);
                };
            } else {
                let hours = formatTimeString(deviceTimer);

                editPageSection.innerHTML = hours;
                row2.onclick = function () {
                    openEditPage(index, this);
                };
                editPageSection.hidden = false;
            }


            row2.appendChild(timerTitle);
            row2.appendChild(editPageSection);

            row1.appendChild(title_wrapper);
            row1.appendChild(label);

            const btnDiv = document.createElement("div");
            btnDiv.classList.add("btn");

            const ind = document.createElement('span');
            ind.className = "btn_index";
            ind.innerText = index + 1;
            const bottom = document.createElement('div');
            ind.className = "btn_index";
            bottom.className = "bottom";
            ind.innerText = index + 1;
            btnDiv.appendChild(ind);
            btnDiv.appendChild(bottom);
            btnDiv.appendChild(row1);
            btnDiv.appendChild(row2);



            return btnDiv;
        }
        function convertHexUnicodeToString(hexUnicode) {
            let result = "";
            for (let i = 0; i < hexUnicode.length; i += 4) {
                let hexChars = hexUnicode.substring(i, i + 4);
                let unicode = String.fromCharCode(parseInt(hexChars, 16));
                result += unicode;
            }
            console.log(result);
            return result;
        }


        class Relay {
            constructor(name, timer, state) {
                this.name = name;
                this.timer = timer;
                this.state = state;
            }
        }

        function onLoaded(index) {
            modelJson(jsonData);
            //let el = document.getElementById("0");
            // el.innerText = el.innerText.substring(8, 18);
        }
        function showTimer(index) {
            let el = document.getElementById(`${index}`);
            el.innerText = el.value.substring(8, 18);
        }

        function closeMyModal(id) {
            let item = document.getElementById(id);
            item.close();
        }

        function formatTimeString(inputString, index) {
            let formattedString;

            if (inputString.length == 18) {
                // 0123456/15:0016:00

                // Extract relevant parts
                const daysOfWeek = inputString.substring(0, 7); // e.g., "0123456"
                const startTime = inputString.substring(8, 13); // e.g., "10:15"
                const endTime = inputString.substring(13); // e.g., "16:30"

                // Map days of the week to their names
                const dayNames = ["Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri"];
                //const formattedDays = daysOfWeek.split("").map(day => dayNames[parseInt(day)]).join(" ");
                // Construct the final formatted string
                const list = document.createElement('ul');
                const container = document.createElement('div');
                const time = document.createElement('span');

                var item;
                daysOfWeek.split("").forEach((day, index) => {
                    item = document.createElement('li');
                    item.innerText = dayNames[index];
                    item.style.color = isNaN(parseInt(day)) ? '#252532' : '#616179';
                    list.appendChild(item);
                });
                container.classList.add("timer_div");
                container.appendChild(list);
                time.innerText = `From: ${startTime} To: ${endTime}`;
                container.appendChild(time);
                list.style.margin = '0px';
                list.style.padding = '0px';


                formattedString = container.innerHTML;


                //formattedString = `${formattedDays}\n From: ${startTime}End: ${endTime}`;

            } else {
                // 0123456/01/00:30

                // Extract relevant parts
                const daysOfWeek = inputString.substring(0, 7); // e.g., "012"
                const per_hour = inputString.substring(8, 10); // e.g., "10:15"
                const onTime = inputString.substring(12); // e.g., "16:30"


                // Map days of the week to their names
                const dayNames = ["Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri"];

                const container = document.createElement('div');
                const time = document.createElement('span');


                // const formattedDays = daysOfWeek
                //   .split("")
                // .map(day => dayNames[parseInt(day)])
                //.join(" ");
                // Construct the final formatted string
                const list = document.createElement('ul');

                var item;
                daysOfWeek.split("").forEach((day, index) => {
                    item = document.createElement('li');
                    item.innerText = dayNames[index];
                    item.style.color = isNaN(parseInt(day)) ? '#252532' : '#616179';
                    list.appendChild(item);

                });

                container.appendChild(list);
                time.innerText = `Per ${parseInt(per_hour)} Hour ${onTime} is On`;
                container.appendChild(time);
                list.style.margin = '0px';
                list.style.padding = '0px';
                formattedString = container.innerHTML;
                //formattedString = `${formattedDays}\n Per: ${parseInt(per_hour)} Hour  ${onTime} On`;

            }


            return formattedString;
        }


        function openEditPage(index, element) {

            var list = document.getElementsByClassName("btn");
            var el = document.getElementsByClassName("timer-title")[index];



            let item = document.getElementById("editor-1");
            let btn = list[index];


            let timerSch = el.value;

            let days = timerSch.substring(0, el.value.indexOf("/")).split("");
            daysInt = [];
            days.forEach((d) => {
                if (true) { //!isNaN(parseInt(d))
                    daysInt.push(parseInt(d));
                }
            });


            if (timerSch.length == 18) {
                let radio = document.getElementById("start-stop");
                radio.checked = true;
            } else if (timerSch.length == 16) {
                let radio = document.getElementById("interval");
                radio.checked = true;
            } else {
                let radio = document.getElementById("off");
                radio.checked = true;
                daysInt = [0, 1, 2, 3, 4, 5, 6];
            }





            console.log(daysInt);

            let daysList = document.getElementById("list");
            daysList.innerHTML = "";

            let l = document.createElement("li");

            weekDays.map((d, index) => {
                const l = document.createElement("li"); // Create a new list item for each day

                if (daysInt.includes(index)) {
                    l.style.color = "#fca311"; // Set the color to red
                } else {
                    l.style.color = "gray"; // Set the color to white
                }

                l.addEventListener("click", function (el) {
                    if (daysInt.includes(index)) {
                        daysInt[index] = NaN;
                        el.srcElement.style.color = "gray";
                    } else {
                        daysInt[index] = index;
                        el.srcElement.style.color = "#fca311";
                    }
                    console.log(daysInt);

                }, false);


                l.appendChild(document.createTextNode(d)); // Add the day name as text
                l.setAttribute("id", "day" + (index + 1)); // Set the ID attribute

                daysList.appendChild(l); // Append the list item to the list
            });


            // hours

            let time = el.value.substring(el.value.indexOf("/") + 1, el.value.length);
            s_h = parseInt(time.substring(0, 2));
            s_m = parseInt(time.substring(3, 5));
            e_h = parseInt(time.substring(5, 7));
            e_m = parseInt(time.substring(8, 10));

            createOptionList("start-hour", 24, s_h);
            createOptionList("start-min", 60, s_m);


            createOptionList("end-hour", 24, e_h);
            createOptionList("end-min", 60, e_m);




            per_h = parseInt(time.substring(0, 2));
            interval_h = parseInt(time.substring(4, 6));
            interval_m = parseInt(time.substring(6, 8));

            createOptionList("per_h", 24, per_h);

            createOptionList("interval_h", 24, interval_h);
            createOptionList("interval_m", 60, interval_m);



            //item.classList.toggle("show");
            //btn.classList.toggle("show");
            selectedId = index;
            item.showModal();


        }
        function setDay(index) {
            console.log("stDay");
            console.log(daysInt);

        }

        function createOptionList(h, count, selected) {
            var parent = document.getElementById(h);

            if (h === 'per_h') {
                let i = 1;
                while (i < 24) {

                    const h = document.createDocumentFragment();
                    h.appendChild(new Option(`${i < 10 ? `0${i}` : i}`, i, i == selected, i == selected));
                    parent.appendChild(h); // Append the list item to the list
                    i = i + 1;
                    while (24 % i != 0) {
                        i = i + 1;
                    }
                }
            } else {
                for (let i = 0; i < count; i++) {
                    const h = document.createDocumentFragment();
                    h.appendChild(new Option(`${i < 10 ? `0${i}` : i}`, i, i == selected, i == selected));
                    parent.appendChild(h); // Append the list item to the list
                }
            }

            parent.setAttribute("selected", selected);

        }

        function toggleCheckbox(element) {
            var xhr = new XMLHttpRequest();
            if (element.checked) { xhr.open("GET", "/update?output=" + element.id + "&state=0", true); }
            else { xhr.open("GET", "/update?output=" + element.id + "&state=1", true); }
            xhr.send();
        }


        function saveTimer(element) {
            console.log(element);
            let ele = document.getElementsByClassName("btn");
            let item = ele[selectedId];
            let i = document.getElementById(`${selectedId}`);
            let str = prepareTimer();
            if (relays[selectedId].timer != str) {

                relays[selectedId].timer = str;
                var xhr = new XMLHttpRequest();
                if (item.innerHTML) { xhr.open("GET", "/update?output=" + selectedId + `&timer=${str}`, true); }
                else { xhr.open("GET", "/update?output=" + selectedId + "&timer=0", true); }
                xhr.send();
                closeMyModal("editor-1");
                showDialog("Timer Saved", 's');
            } else {
                closeMyModal("editor-1");
            }

        }
        function prepareTimer() {

            let str = "";
            //days
            for (let i = 0; i < daysInt.length; i++) {
                if (Number.isInteger(daysInt[i])) {
                    str += daysInt[i];
                } else {
                    str += "*";
                }
            }
            // hours
            let radio_interval = document.getElementById("interval");
            let start_stop = document.getElementById("start-stop");

            if (radio_interval.checked) {
                var per_hour = document.getElementById("per_h");
                let interval_hour = document.getElementById("interval_h");
                let interval_min = document.getElementById("interval_m");


                per_h = per_hour.options[per_hour.selectedIndex].value;
                interval_h = interval_hour.options[interval_hour.selectedIndex].value;
                interval_m = interval_min.options[interval_min.selectedIndex].value;

                str += "/";

                str += per_h > 9 ? per_h : `0${per_h}`;
                str += "/";
                str += interval_h > 9 ? interval_h : `0${interval_h}`;

                str += ":";
                str += interval_m > 9 ? interval_m : `0${interval_m}`;
                console.log(str);
                return str;
            } else if (start_stop.checked) {
                var start_hour = document.getElementById("start-hour");
                let start_min = document.getElementById("start-min");
                let end_hour = document.getElementById("end-hour");
                let end_min = document.getElementById("end-min");

                s_h = start_hour.options[start_hour.selectedIndex].value;
                s_m = start_min.options[start_min.selectedIndex].value;
                e_h = end_hour.options[end_hour.selectedIndex].value;
                e_m = end_min.options[end_min.selectedIndex].value;

                str += "/";

                str += s_h > 9 ? s_h : `0${s_h}`;
                str += ":";
                str += s_m > 9 ? s_m : `0${s_m}`;

                str += e_h > 9 ? e_h : `0${e_h}`;
                str += ":";
                str += e_m > 9 ? e_m : `0${e_m}`;
                console.log(str);
                return str;
            } else {

                return "";
            }

        }
        String.prototype.replaceAt = function (index, replacement) {
            return this.substring(0, index) + replacement + this.substring(index + replacement.length);
        }
         function createSignal(signal) {
            let count = 0;
            const ant = document.createElement("div");
            ant.classList.add("antenna") ;
            ant.id = "antenna";
            if(signal == 0 || signal >34){
                const sec = document.createElement("li");
                sec.innerText = "x";
                sec.style.color = "red";
                ant.style.paddingBottom = "0px";
                ant.appendChild(sec);

            }else{
                for (let i = 0; i < 6; i++) {
                    const sec = document.createElement("li");
                        count = i*5;
                    sec.style.height = `${(i * 3)}px`;
                    sec.style.width = "2px";
                    if(  signal <count){
                        sec.style.backgroundColor = "#3c3950";
                    }else{
                        sec.style.backgroundColor = "white";
                    }

                    ant.appendChild(sec);
                }
            }
            const signalEl = document.getElementById("gsm");
             try {
                signalEl.removeChild(signalEl.getElementById("antenna"));
            } catch (e) {
                console.log(e);
            }
            signalEl.appendChild(ant);
        }
        function toggleTimer(element) {
            var xhr = new XMLHttpRequest();
            if (element.innerHTML) { xhr.open("GET", "/update?output=" + element.id + "&timer=1", true); }
            else { xhr.open("GET", "/update?output=" + element.id + "&timer=0", true); }
            xhr.send();
        }
        if (!!window.EventSource) {
            var source = new EventSource('/events');

            source.addEventListener('open', function (e) {
                console.log("Events Connected");
            }, false);
            source.addEventListener('error', function (e) {
                if (e.target.readyState != EventSource.OPEN) {
                    console.log("Events Disconnected");
                }
            }, false);

            source.addEventListener('message', function (e) {
                console.log("message", e.data);
            }, false);

            source.addEventListener('gsm', function (e) {
                console.log("gsm", e.data);
                document.getElementById("gsm").innerHTML = e.data;
            }, false);
            source.addEventListener('sensor_1', function (e) {
                console.log("sensor_1", e.data);
                document.getElementById("sensor_1").innerHTML = e.data;
            }, false);
            source.addEventListener('signal', function (e) {
                console.log("signal", e.data);
                createSignal(e.data);
            }, false);
              source.addEventListener('data', function (e) {
                console.log("data", e.data);
                modelJson(JSON.parse(e.data));
            }, false);
                source.addEventListener('refresh', function (e) {
                console.log("refresh", e.data);
                    location.reload();
            }, false);
        }




    </script>
</body>

</html>
)rawliteral";