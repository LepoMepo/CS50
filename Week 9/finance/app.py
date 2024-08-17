import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
import time

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    shares = []
    sum_total = 0
    list_of_shares = db.execute("SELECT DISTINCT(share) FROM operations WHERE username_id=?", session.get("user_id"))
    for share in list_of_shares:
        number_of_shares = db.execute("SELECT SUM(number_of_shares) FROM operations WHERE share=? AND username_id=?", share["share"], session.get("user_id"))
        sum_share = 0
        if number_of_shares[0]["SUM(number_of_shares)"] != 0:
            share_info = lookup(share["share"])
            sum_share = number_of_shares[0]["SUM(number_of_shares)"] * share_info["price"]
            sum_total += sum_share
            shares.append({"symbol": share["share"], "shares": number_of_shares[0]["SUM(number_of_shares)"], "price": usd(share_info["price"]), "total": usd(sum_share)})
    cash_dict = db.execute("SELECT cash FROM users WHERE id=?", session.get("user_id"))
    cash = cash_dict[0]["cash"]
    sum_total += cash
    return render_template("index.html", shares=shares, cash=usd(cash), total=usd(sum_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        quote = request.form.get("symbol")
        if not quote:
            return apology("You forget to enter Symbol!")
        share_info = lookup(quote)
        if share_info is None:
            return apology("Enter correct Symbol!")

        shares = request.form.get("shares")
        if not shares:
            return apology("You forget to enter number of shares!")
        try:
            shares = int(shares)
        except ValueError:
            return apology("You should enter a positive integer in Shares!")
        if shares < 1:
            return apology("You should enter a positive integer in Shares!")

        sum = share_info["price"] * shares
        money = db.execute("SELECT cash FROM users WHERE id=?", session.get("user_id"))
        if sum > money[0]["cash"]:
            return apology("You don't have enough money")
        else:
            db.execute("UPDATE users SET cash=? WHERE id=?", money[0]["cash"] - sum, session.get("user_id"))
            t = time.localtime()
            current_time = time.strftime("%d.%m.%Y %H:%M:%S", t)
            db.execute("INSERT INTO operations (username_id, share, price, date, type, share_price, number_of_shares) VALUES(?, ?, ?, ?, ?, ?, ?)",
                       session.get("user_id"), share_info["name"], sum, current_time, "BUY", share_info["price"], shares)
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    shares = db.execute("SELECT share, number_of_shares, share_price, date FROM operations WHERE username_id=?", session.get("user_id"))
    return render_template("history.html", shares=shares)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        quote = request.form.get("symbol")
        result = lookup(quote)
        if result is None:
            return apology("Bad Boy")
        return render_template("quoted.html", quote=result)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("Enter the username!")
        if not password:
            return apology("Enter the password!")
        if not confirmation:
            return apology("Enter the password again!")
        if password != confirmation:
            return apology("Passwords must match!")

        try:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))
        except ValueError:
            return apology("You have already registered")

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    symbols = db.execute("SELECT DISTINCT(share) FROM operations WHERE username_id=?", session.get("user_id"))
    ls_symbols = []
    for symbol_ in symbols:
        sum = db.execute("SELECT SUM(number_of_shares) FROM operations WHERE share=? and username_id=?", symbol_["share"], session.get("user_id"))
        if sum[0]["SUM(number_of_shares)"] != 0:
            ls_symbols.append(symbol_["share"])

    if request.method == "POST":
        symbol = request.form.get("symbol")
        print(symbol)
        if not symbol:
            return apology("You must choose the Symbol!")
        if symbol not in ls_symbols:
            return apology("Invalid symbol")

        shares = request.form.get("shares")
        if not shares:
            return apology("Shares must be positive integer")
        try:
            shares = int(shares)
        except ValueError:
            return apology("Shares must be positive integer")
        shares_sum = db.execute("SELECT SUM(number_of_shares) FROM operations WHERE share=? AND username_id=?", symbol, session.get("user_id"))
        if shares > shares_sum[0]["SUM(number_of_shares)"] or shares < 1:
            return apology("Shares must be positive integer")

        share_info = lookup(symbol)
        sum = share_info["price"] * shares
        money = db.execute("SELECT cash FROM users WHERE id=?", session.get("user_id"))
        db.execute("UPDATE users SET cash=? WHERE id=?", money[0]["cash"] + sum, session.get("user_id"))
        t = time.localtime()
        current_time = time.strftime("%d.%m.%Y %H:%M:%S", t)
        db.execute("INSERT INTO operations (username_id, share, price, date, type, share_price, number_of_shares) VALUES(?, ?, ?, ?, ?, ?, ?)",
                       session.get("user_id"), share_info["name"], sum, current_time, "SELL", share_info["price"], -shares)
        return redirect("/")
    else:
        return render_template("sell.html", symbols=ls_symbols)
