import os

import datetime
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

MONTHS_LEAP = {
    1:  31,
    2:  29,
    3:  31,
    4:  30,
    5:  31,
    6:  30,
    7:  31,
    8:  31,
    9:  30,
    10: 31,
    11: 30,
    12: 31
}

MONTHS_NOT_LEAP = {
    1:  31,
    2:  28,
    3:  31,
    4:  30,
    5:  31,
    6:  30,
    7:  31,
    8:  31,
    9:  30,
    10: 31,
    11: 30,
    12: 31
}

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        if not name:
            return render_template("failure.html", message="You forget to put name")

        day = request.form.get("day")
        if not day:
            return render_template("failure.html", message="You forget to put day")
        else:
            day = int(day)

        month = request.form.get("month")
        if not month:
            return render_template("failure.html", message="You forget to put month")
        else:
            month = int(month)

        year = request.form.get("year")
        if not year:
            return render_template("failure.html", message="You forget to put year")
        else:
            year = int(year)

        if year%4==0 and (year%100!=0 or year%400==0):
            if month not in MONTHS_LEAP.keys() or day < 1 or day > MONTHS_LEAP[month]:
                return render_template("failure.html", message="You entered wrong date")
        else:
            if month not in MONTHS_NOT_LEAP.keys() or day < 1 or day > MONTHS_NOT_LEAP[month]:
                return render_template("failure.html", message="You entered wrong date")

        db.execute("INSERT INTO birthdays (name, month, day, year) VALUES(?, ?, ?, ?)", name, month, day, year)
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays")
        for birthday in birthdays:
            date = datetime.date(birthday["year"], birthday["month"], birthday["day"])
            birthday["date"] = date
        return render_template("index.html", birthdays=birthdays)

@app.route("/delete", methods=["POST"])
def delete():
    id = request.form.get("id")
    if id:
        db.execute("DELETE FROM birthdays WHERE id=?", id)
    return redirect("/")
