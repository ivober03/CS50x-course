import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from sqlalchemy import text
from datetime import datetime

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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

    # select user's data (portfolio and cash)
    rows = db.execute("SELECT * FROM portfolio WHERE user_id = :id", id=session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

    # select cash balance
    cash = cash[0]['cash']
    total = cash

    # select stock's data (name, value and quantity)
    for row in rows:
        dict = lookup(row['symbol'])
        row['name'] = dict['name']
        row['price'] = dict['price']
        row['total'] = row['price'] * row['shares']

        # add stock's value to the balance
        total += row['total']


    return render_template("index.html", rows=rows, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # saves stock symbol and shares
        lkup_symbol = lookup(request.form.get("symbol"))
        new_shares = (request.form.get("shares"))

        # ensure shares is not fractional or non-numeric
        if not new_shares.isdigit():
            return apology("cannot purchase partial shares", 400)

        new_shares = int(new_shares)

        # ensure symbol is valid
        if lkup_symbol == None:
            return apology("invalid stock symbol", 400)

        # ensure shares quantity
        if new_shares <= 0:
            return apology("invalid shares quantity", 400)

        # save transaction price
        transaction = lkup_symbol['price'] * new_shares

        # ensure user's cash balance is enough
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        cash = cash[0]['cash']
        cash_left = cash - transaction

        if cash_left < 0:
            return apology("insufficient funds", 400)

        # query portfolio table for userid and symbol
        symbol = lkup_symbol['symbol']
        row = db.execute("SELECT * FROM portfolio WHERE user_id = :id AND symbol = :symbol", id=session["user_id"], symbol=symbol)

        # if there is no row, create a new one with the purchased shares
        if len(row) != 1:
            db.execute("INSERT INTO portfolio (user_id, symbol, shares) VALUES (:id, :symbol, :new_shares)", id=session["user_id"], symbol=symbol, new_shares=new_shares)

        # if there is already a portfolio for the user and symbol, update shares
        else:
            db.execute("UPDATE portfolio SET shares = shares + :new_shares WHERE user_id = :id", new_shares=new_shares, id=session["user_id"])

        # update user's cash balance
        db.execute("UPDATE users SET cash = :cash_left WHERE id =:id", cash_left=cash_left, id=session["user_id"])

        # update history table

        # save date and time
        current_datetime = datetime.now()
        formatted_datetime = current_datetime.strftime("%Y-%m-%d %H:%M:%S")

        db.execute("INSERT INTO history (user_id, symbol, shares, type, price, date) VALUES (:userid, :symbol, :shares, 'Buy', :price, :date)",
           userid=session["user_id"], symbol=symbol, shares=new_shares, price=lkup_symbol['price'], date=formatted_datetime)

        flash(f"{new_shares} shares added to your portfolio")

        # redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # select history's data
    rows = db.execute("SELECT * FROM history WHERE user_id = :id", id=session["user_id"])

    return render_template("history.html", rows=rows)


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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # lookup stock symbol
        symbol = lookup(request.form.get("symbol"))

        # ensure symbol is valid
        if symbol == None:
            return apology("invalid stock symbol", 400)

        # display the results back
        return render_template("quoted.html", symbol=symbol)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # lookup stock symbol
        cash_add = request.form.get("cash")
        cash_add_int = int(cash_add)
        # ensure cash is not fractional or non-numeric
        if not cash_add.isdigit() or cash_add_int < 0:
            return apology("cash can only be a positive integer", 400)

        # update user's cash balance
        db.execute("UPDATE users SET cash = cash + :cash_add WHERE id =:id", cash_add=cash_add_int, id=session["user_id"])

        # save date and time
        current_datetime = datetime.now()
        formatted_datetime = current_datetime.strftime("%Y-%m-%d %H:%M:%S")

        db.execute("INSERT INTO history (user_id, type, price, date) VALUES (:userid, 'Cash deposit', :cash_add, :date)",
           userid=session["user_id"], cash_add=cash_add_int, date=formatted_datetime)

        flash(f"{cash_add_int} USD added to your account")

        # redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("cash.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure passwords match
        elif (request.form.get("password") != request.form.get("confirmation")):
            return apology("passwords must match", 400)

        # save user data
        username = request.form.get("username")
        password_hash = generate_password_hash(request.form.get("password"))

        # ensure username is not in the database
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)

        if len(rows) != 0:
            return apology("username is already taken", 400)

        # insert user data into the database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=password_hash)

        flash("Welcome to your portfolio!")

        # Redirect user to login form
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # saves stock symbol and shares
        symbol = (request.form.get("symbol"))
        shares_sold = (request.form.get("shares"))
        shares_sold = int(shares_sold)

        # select user's data from the portfolio
        rows = db.execute("SELECT * FROM portfolio WHERE user_id = :id AND symbol = :symbol", id=session["user_id"], symbol=symbol)

        # ensure symbol is valid
        if symbol == None or len(rows) != 1:
            return apology("invalid stock symbol", 400)

        # save shares number
        old_shares = rows[0]['shares']

        # ensure shares quantity
        if shares_sold < 0 or shares_sold > old_shares:
            return apology("invalid shares quantity", 400)

        # save current stock price
        lkup_symbol = lookup(symbol)
        transaction = lkup_symbol['price'] * shares_sold

        # update user's cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        cash = cash[0]['cash']
        cash = cash + transaction

        # update user's cash balance
        db.execute("UPDATE users SET cash = :cash WHERE id =:id", cash=cash, id=session["user_id"])

        # update shares quantity
        shares = old_shares - shares_sold
        if shares > 0:
            db.execute("UPDATE portfolio SET shares = :shares WHERE symbol = :symbol and user_id = :id", shares=shares, symbol=symbol, id=session["user_id"])
        else:
            db.execute("DELETE FROM portfolio WHERE symbol = :symbol AND user_id = :id",
                       symbol=symbol, id=session["user_id"])

        # update history table

        # save date and time
        current_datetime = datetime.now()
        formatted_datetime = current_datetime.strftime("%Y-%m-%d %H:%M:%S")

        db.execute("INSERT INTO history (user_id, symbol, shares, type, price, date) VALUES (:userid, :symbol, :shares, 'Sell', :price, :date)",
           userid=session["user_id"], symbol=symbol, shares=shares_sold, price=lkup_symbol['price'], date=formatted_datetime)

        flash(f"{shares_sold} shares sold")

        # redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        # select user's data from the portfolio
        rows = db.execute("SELECT * FROM portfolio WHERE user_id = :id", id=session["user_id"])

        # select stock's name
        for row in rows:
            dict = lookup(row['symbol'])
            row['name'] = dict['name']

        return render_template("sell.html", rows=rows)
