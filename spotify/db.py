import psycopg2
import psycopg2.extras

def connect():
    conn = psycopg2.connect(
        dbname='projet_s1',
        user='marie',
        password='marie',
        host='localhost',  
        port='5432', 
        cursor_factory=psycopg2.extras.NamedTupleCursor,
    )
    conn.autocommit = True 
    return conn
    