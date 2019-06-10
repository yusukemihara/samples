module A

  module_function

  def a(text)
    puts text
  end

  def test(text)
    __send__ "a",text
  end

end

A.test("hoge")
