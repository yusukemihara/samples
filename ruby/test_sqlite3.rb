#!/bin/ruby

require "sqlite3"
require "test/unit"

class TestSqlite3 < Test::Unit::TestCase
  def setup
    @db = SQLite3::Database.new(":memory:")
    @db.type_translation = true
    @db.execute(<<-SQL)
      CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT);
    SQL
  end

  def test_case1
    @db.execute_batch(<<-SQL)
      CREATE TABLE foo(id INTEGER PRIMARY KEY, name TEXT);
      CREATE TABLE bar(id INTEGER PRIMARY KEY, name TEXT);
    SQL
    p @db.execute("SELECT tbl_name FROM sqlite_master WHERE TYPE=?", "table").flatten
    @db.execute_batch(<<-SQL)
      INSERT INTO foo VALUES(0,NULL);
      INSERT INTO foo VALUES(1,"");
    SQL
    p @db.execute("SELECT * FROM foo")
  end

  VERSION=255
  def test_case2
      @db.execute_batch(<<-SQL)
        CREATE TABLE system(version INTEGER PRIMARY KEY);
        CREATE TABLE control(
          name        TEXT PRIMARY KEY NOT NULL,
          version     TEXT NOT NULL,
          description TEXT,
          maintainer  TEXT,
          install     TEXT,
          link        TEXT
        );
        CREATE TABLE dependence(
          name        TEXT,
          depend      TEXT NOT NULL,
          version     TEXT,
          type        TEXT
        );
        INSERT INTO system values (#{VERSION});
      SQL
    @db.execute("INSERT INTO dependence VALUES(?,?,?,?)",nil,"hoge","moge","age")
    p @db.execute("SELECT * FROM dependence")
  end

  def test_case3
    @db.execute_batch(<<-SQL,1,"hoge")
      INSERT INTO test values(?,?)
    SQL
    p @db.execute("SELECT * FROM test")
  end

  def test_case4
    @db.execute("INSERT INTO test VALUES(?,?)",0,"hoge")
    @db.execute("INSERT INTO test VALUES(?,?)",1,"noge")
    @db.execute("INSERT INTO test VALUES(?,?)",2,"age")
    @db.execute("INSERT INTO test VALUES(?,?)",3,"age")
    @db.execute("INSERT INTO test VALUES(?,?)",4,"age")
    @db.execute("INSERT INTO test VALUES(?,?)",5,"age")
    p @db.execute("SELECT * FROM test")
    @db.execute("DELETE FROM test WHERE name=?","age")
    p @db.execute("SELECT * FROM test")
  end

  def test_case5
    @db.execute("INSERT INTO test VALUES(?,?)",1,"noge")
    p @db.execute("SELECT name FROM test WHERE name='noge'")
  end

  def test_case6
    @db.execute("INSERT INTO test VALUES(?,?)",1,"noge")
    p @db.execute("SELECT name FROM test WHERE name LIKE ?", "n%")
    p @db.execute("SELECT * FROM test")
  end

  def test_case7
    p @db.execute("INSERT INTO test VALUES(?,?)",1,"noge")
    p @db.execute("UPDATE test SET name=? WHERE id=?;","TRUE",1)
    p @db.execute("UPDATE test SET name=? WHERE id=?;","TRUE",5)
    p @db.execute("SELECT * FROM test")
  end

  def test_case8
    @db.execute_batch(<<-SQL)
      CREATE TABLE foo(id INTEGER PRIMARY KEY, name TEXT);
    SQL
    @db.execute_batch(<<-SQL)
      INSERT INTO foo (name) VALUES("a");
      INSERT INTO foo (name) VALUES("b");
      INSERT INTO foo (name) VALUES("c");
    SQL
    p @db.execute("SELECT * FROM foo")
  end

  def test_case9
    @db.execute_batch(<<-SQL)
      CREATE TABLE foo(id INTEGER PRIMARY KEY, name TEXT);
      CREATE TABLE bar(id INTEGER, id2 INTEGER);
    SQL
    @db.execute_batch(<<-SQL)
      INSERT INTO foo (name) VALUES("a");
      INSERT INTO foo (name) VALUES("b");
      INSERT INTO foo (name) VALUES("c");
      INSERT INTO bar VALUES(1,3);
      INSERT INTO bar VALUES(2,3);
    SQL
    p @db.execute("SELECT * FROM foo")
    p @db.execute("SELECT * FROM bar")
    p @db.execute("SELECT name FROM foo WHERE id IN (SELECT id FROM bar WHERE id2 = 3)")
  end

  def teardown
    @db.close
  end
end
