class EmptyVST
  def set_parameter(index, value)
  end

  def parameter_name(index)
    'empty'
  end

  def parameter_value(index)
    0.0
  end

  def parameter_display_value(index)
    '0'
  end

  def parameter_label(index)
  end

  def process(data)
    data
  end
end

EmptyVST.new
