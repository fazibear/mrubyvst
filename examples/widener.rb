class VST
  attr_reader :vendor, :product, :version

  def initialize
    @vendor  = 'MRuby powered plugin'
    @product = 'Stereo Widener Plugin'
    @name    = 'Stereo Widener Vst'
    @version = 0
    @width   = 0
    @mid     = 1
    @side    = 1
  end

  def parameter_set(index, value)
    if index == 0
      @width = value
    end
  end

  def parameter_get(index)
    @pan if index == 0
  end

  def parameter_name(index)
    if index == 0
      "Width"
    else
      "empty"
    end
  end

  def parameter_display(index)
    if index == 0
      (@width * 10).to_i.to_s
    else
      ''
    end
  end

  def parameter_label(index)
    if index == 0
      ''
    else
      ''
    end
  end

  def process_replacing(data)
    length = data[0].length
    length.times do |t|
      data[0][t], data[1][t] = process(data[0][t], data[1][t])
    end
    data
  end

  def process(l,r)
    mid  = 0.5 * (l + r)
    side = 0.5 * (r - l) * @width
    [mid - side, mid + side]
  end
end
