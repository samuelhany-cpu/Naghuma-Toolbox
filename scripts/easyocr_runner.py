"""
EasyOCR Runner Script
Performs OCR on an image and returns results as JSON
"""
import sys
import json
import os

def run_ocr(image_path):
    try:
        import easyocr
        import cv2
        import numpy as np
        
        # Create reader (will download model on first run)
        reader = easyocr.Reader(['en'], gpu=False)  # English only, CPU mode
        
        # Read image
        image = cv2.imread(image_path)
        if image is None:
            return {
                "success": False,
                "error": f"Failed to read image: {image_path}",
                "text": "",
                "confidence": 0.0
            }
        
        # Perform OCR
        results = reader.readtext(image)
        
        # Extract text and calculate average confidence
        all_text = []
        total_conf = 0.0
        
        for (bbox, text, conf) in results:
            all_text.append(text)
            total_conf += conf
        
        combined_text = ' '.join(all_text)
        avg_confidence = (total_conf / len(results) * 100) if results else 0.0
        
        return {
            "success": True,
            "text": combined_text,
            "confidence": avg_confidence,
            "error": ""
        }
        
    except ImportError as e:
        return {
            "success": False,
            "error": f"EasyOCR not installed. Install with: pip install easyocr",
            "text": "",
            "confidence": 0.0
        }
    except Exception as e:
        return {
            "success": False,
            "error": f"OCR Error: {str(e)}",
            "text": "",
            "confidence": 0.0
        }

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(json.dumps({
            "success": False,
            "error": "Usage: python easyocr_runner.py <image_path>",
            "text": "",
            "confidence": 0.0
        }))
        sys.exit(1)
    
    image_path = sys.argv[1]
    
    if not os.path.exists(image_path):
        print(json.dumps({
            "success": False,
            "error": f"Image file not found: {image_path}",
            "text": "",
            "confidence": 0.0
        }))
        sys.exit(1)
    
    result = run_ocr(image_path)
    print(json.dumps(result))
