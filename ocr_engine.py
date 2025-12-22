#!/usr/bin/env python3
"""
EasyOCR Engine for Naghuma Toolbox
Provides OCR functionality via EasyOCR library
"""

import sys
import json
import os
from pathlib import Path

def perform_ocr(image_path):
    """
    Perform OCR on the given image using EasyOCR
    Returns JSON with text and confidence
    """
    try:
        import easyocr
        
        # Create reader (English only for now)
        # gpu=False for CPU-only, set to True if you have CUDA
        reader = easyocr.Reader(['en'], gpu=False, verbose=False)
        
        # Perform OCR
        results = reader.readtext(image_path)
        
        # Process results
        text_lines = []
        total_confidence = 0
        
        for (bbox, text, conf) in results:
            text_lines.append(text)
            total_confidence += conf
        
        # Calculate average confidence
        avg_confidence = (total_confidence / len(results) * 100) if results else 0
        
        # Combine all text
        full_text = '\n'.join(text_lines)
        
        # Return JSON result
        result = {
            'success': True,
            'text': full_text,
            'confidence': round(avg_confidence, 2),
            'line_count': len(text_lines)
        }
        
        print(json.dumps(result))
        return 0
        
    except ImportError:
        error = {
            'success': False,
            'error': 'EasyOCR not installed. Install with: pip install easyocr',
            'text': '',
            'confidence': 0
        }
        print(json.dumps(error))
        return 1
        
    except Exception as e:
        error = {
            'success': False,
            'error': str(e),
            'text': '',
            'confidence': 0
        }
        print(json.dumps(error))
        return 1

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(json.dumps({
            'success': False,
            'error': 'Usage: python ocr_engine.py <image_path>',
            'text': '',
            'confidence': 0
        }))
        sys.exit(1)
    
    image_path = sys.argv[1]
    
    if not os.path.exists(image_path):
        print(json.dumps({
            'success': False,
            'error': f'Image file not found: {image_path}',
            'text': '',
            'confidence': 0
        }))
        sys.exit(1)
    
    sys.exit(perform_ocr(image_path))
