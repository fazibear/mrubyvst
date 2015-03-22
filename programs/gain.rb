class GainVST
  def initialize
    @gain = 1.0
  end

  def set_parameter(index, value)
    @gain = value if index = 0
  end

  def parameter_name(index)
    index == 0 ? 'Gain' : 'empty'
  end

  def parameter_value(index)
    @gain if index == 0
  end

  def parameter_display_value(index)
    @gain.to_s if index == 0
  end

  def parameter_label(index)
    'dB' if index == 0
  end

  def process(data)
    data[0].map!{ |left | left * @gain}
    data[1].map!{ |right| right * @gain }
    data
  end
end

GainVST.new
