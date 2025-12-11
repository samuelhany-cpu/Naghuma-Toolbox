import { Eye, EyeOff, Image } from 'lucide-react';

interface Layer {
  id: string;
  name: string;
  visible: boolean;
  opacity: number;
}

interface LayersTabProps {
  layers: Layer[];
  onLayerToggle: (id: string) => void;
  onOpacityChange: (id: string, opacity: number) => void;
}

export function LayersTab({ layers, onLayerToggle, onOpacityChange }: LayersTabProps) {
  return (
    <div className="p-4 space-y-3">
      <h3 className="text-fuchsia-300">Layers</h3>
      
      {layers.map((layer) => (
        <div
          key={layer.id}
          className="bg-purple-900/20 border border-purple-800/30 rounded-lg p-3 hover:border-fuchsia-500/50 transition-colors"
        >
          <div className="flex items-center justify-between mb-2">
            <div className="flex items-center gap-2">
              <Image className="w-4 h-4 text-fuchsia-400" />
              <span className="text-sm text-purple-100">{layer.name}</span>
            </div>
            <button
              onClick={() => onLayerToggle(layer.id)}
              className="p-1 hover:bg-purple-800/50 rounded transition-colors"
            >
              {layer.visible ? (
                <Eye className="w-4 h-4 text-fuchsia-400" />
              ) : (
                <EyeOff className="w-4 h-4 text-purple-400/50" />
              )}
            </button>
          </div>
          
          <div className="space-y-1">
            <div className="flex items-center justify-between">
              <label className="text-purple-300">Opacity</label>
              <span className="text-xs text-purple-300 bg-purple-900/40 px-2 py-1 rounded">
                {layer.opacity}%
              </span>
            </div>
            <input
              type="range"
              min="0"
              max="100"
              value={layer.opacity}
              onChange={(e) => onOpacityChange(layer.id, Number(e.target.value))}
              disabled={!layer.visible}
              className="w-full h-1.5 bg-purple-900/40 rounded-lg appearance-none cursor-pointer slider"
              style={{
                background: `linear-gradient(to right, #e879f9 0%, #e879f9 ${layer.opacity}%, #3d3250 ${layer.opacity}%, #3d3250 100%)`
              }}
            />
          </div>
        </div>
      ))}

      <style jsx>{`
        .slider::-webkit-slider-thumb {
          appearance: none;
          width: 12px;
          height: 12px;
          background: linear-gradient(135deg, #e879f9, #c026d3);
          border-radius: 50%;
          cursor: pointer;
          box-shadow: 0 0 6px rgba(232, 121, 249, 0.5);
        }

        .slider::-moz-range-thumb {
          width: 12px;
          height: 12px;
          background: linear-gradient(135deg, #e879f9, #c026d3);
          border-radius: 50%;
          cursor: pointer;
          border: none;
          box-shadow: 0 0 6px rgba(232, 121, 249, 0.5);
        }

        .slider:disabled {
          opacity: 0.5;
          cursor: not-allowed;
        }
      `}</style>
    </div>
  );
}
