class VST
  attr_reader :vendor, :product, :version

  def initialize
    @vendor  = 'MRuby powered plugin'
    @product = 'Gain Plugin'
    @name    = 'Gain Vst'
    @version = 0
    @gain    = 1.0
  end

  def parameter_set(index, value)
    @gain = value if index == 0
  end

  def parameter_get(index)
    @gain if index == 0
  end

  def parameter_name(index)
    if index == 0
      "Gain"
    else
      "empty"
    end
  end

  def parameter_display(index)
    if index == 0
      @gain.to_s
    else
      ''
    end
  end

  def parameter_label(index)
    if index == 0
      'dB'
    else
      ''
    end
  end

  def process_replacing(data)
    data[0].map!{ |left | left  * @gain }
    data[1].map!{ |right| right * @gain }
    data
  end
end
