// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// Display performance test for KYWY library
// Tests update rate vs actual frame rate including callback-driven frame sends 
// Does not test display quality, only performance of the library under stress

#include "Kywy.hpp"
#include "my_animation.hpp"

Kywy::Engine engine;

int testPhase = -1; // Start with strobe warning
unsigned long phaseStartTime = 0;
int frameCount = 0;
int updateCount = 0;
int successfulFrames = 0;
int testCycle = 1;

void setup() {
  Serial.begin(9600);
  delay(2000); // Wait for serial connection
  
  Serial.println("=== Display Performance Test ===");
  Serial.println("Tracking update rate vs actual frame rate");
  Serial.println("Including callback-driven frame sends");
  engine.start();
  phaseStartTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - phaseStartTime;
  
  switch (testPhase) {
    case -1: // Strobe warning phase
      {
        engine.display.clear();
        engine.display.drawText(5, 5, "KYWY DISPLAY TEST");
        engine.display.drawText(5, 25, "WARNING!");
        engine.display.drawText(5, 45, "FLASHING LIGHTS");
        engine.display.drawText(5, 65, "STARTING IN:");
        engine.display.drawText(5, 85, String(String(5 - (elapsed / 1000)) + " seconds").c_str());
        
        engine.display.update();
        
        if (elapsed > 5000) {
          testPhase = 0;
          phaseStartTime = currentTime;
          frameCount = 0;
          updateCount = 0;
          successfulFrames = 0;
          engine.display.resetPerformanceCounters();
          Serial.print("Starting test cycle ");
          Serial.println(testCycle);
        }
        break;
      }
      
    case 0: // Dropped frame test - grid pattern with varying delays, gaps in grid show update errors
      {
        engine.display.clear();
        engine.display.drawText(0, 0, "1: Drop Test");
        
        // Draw grid of squares with varying delays
        int squareSize = 8;
        int gridCols = KYWY_DISPLAY_WIDTH / squareSize;
        int gridRows = (KYWY_DISPLAY_HEIGHT - 15) / squareSize; // Leave space for text
        
        for (int row = 0; row < gridRows; row++) {
          for (int col = 0; col < gridCols; col++) {
            int x = col * squareSize;
            int y = 15 + row * squareSize; // Start below text
            
            // Alternate filled/empty squares in checkerboard pattern
            if ((row + col + frameCount) % 2 == 0) {
              engine.display.fillRectangle(x, y, squareSize-1, squareSize-1);
            } else {
              engine.display.drawRectangle(x, y, squareSize-1, squareSize-1);
            }
            delay(row);
            engine.update();
            updateCount++;
          }
          
          // Update after each row with varying delays
          delay(50);
          engine.display.update();
          updateCount++;
          
          // Check if the frame was actually sent
          if (engine.display.checkPendingUpdate()) {
            successfulFrames++;
          }
        }
        
        frameCount++;
        
        // Switch to animation test after 3 seconds
        if (elapsed > 3000) {
          testPhase = 1;
          phaseStartTime = currentTime;
          frameCount = 0;
          updateCount = 0;
          successfulFrames = 0;
          
          Serial.println("Phase 1 complete");
          Serial.print("  Update rate: ");
          Serial.print(updateCount * 1000.0 / elapsed);
          Serial.println(" updates/sec");
          Serial.print("  Frame rate: ");
          Serial.print(successfulFrames * 1000.0 / elapsed);
          Serial.println(" frames/sec");
          Serial.print("  Drop rate: ");
          Serial.print(((updateCount - successfulFrames) * 100.0) / updateCount);
          Serial.println("%");
        }
        break;
      }
      
    case 1: // Fast animation test
      {
        engine.display.clear();
        engine.display.drawText(0, 0, "2: Animation Test");
        
        // Bouncing ball animation
        static int ballX = 20, ballY = 30;
        static int velX = 2, velY = 1;
        
        ballX += velX;
        ballY += velY;
        
        // Bounce off edges
        if (ballX <= 0 || ballX >= KYWY_DISPLAY_WIDTH - 10) velX = -velX;
        if (ballY <= 20 || ballY >= KYWY_DISPLAY_HEIGHT - 10) velY = -velY;
        
        // Draw ball
        engine.display.fillCircle(ballX, ballY, 10);
        
        // Draw rotating line
        float angle = (frameCount * 0.1);
        int centerX = KYWY_DISPLAY_WIDTH - 30;
        int centerY = 40;
        int lineX = centerX + cos(angle) * 15;
        int lineY = centerY + sin(angle) * 15;
        engine.display.drawLine((int16_t)centerX, (int16_t)centerY, (int16_t)lineX, (int16_t)lineY);
        
        engine.display.update();
        updateCount++;
        
        // Check if the frame was actually sent
        if (engine.display.checkPendingUpdate()) {
          successfulFrames++;
        }
        
        frameCount++;
        
        // Switch to FPS test after 5 seconds
        if (elapsed > 5000) {
          testPhase = 2;
          phaseStartTime = currentTime;
          frameCount = 0;
          updateCount = 0;
          successfulFrames = 0;
          engine.display.resetPerformanceCounters(); // Reset callback counters for clean FPS measurement
          
          Serial.println("Phase 2 complete");
          Serial.print("  Update rate: ");
          Serial.print(updateCount * 1000.0 / elapsed);
          Serial.println(" updates/sec");
          Serial.print("  Frame rate: ");
          Serial.print(successfulFrames * 1000.0 / elapsed);
          Serial.println(" frames/sec");
        }
        break;
      }
      
    case 2: // FPS measurement test - no serial output during test
      {
        engine.display.clear();
        engine.display.drawText(0, 0, String(String(testCycle) + ": UPS vs FPS Test").c_str());
        
        // Get callback counters
        int callbackSends = engine.display.getTotalCallbackSends();
        int callbackSuccessful = engine.display.getTotalSuccessfulSends();
        int totalRealFrames = successfulFrames + callbackSuccessful;
        int totalAttempts = updateCount + callbackSends;
        
        // Calculate exact 1-second rolling averages
        static unsigned long lastFPSTime = 0;
        static int lastUpdateCount = 0;
        static int lastSuccessfulFrames = 0;
        static int lastCallbackSends = 0;
        static int lastCallbackSuccessful = 0;
        static float updateRate = 0;
        static float frameRate = 0;
        
        if (currentTime - lastFPSTime >= 1000) { // Update every 1 second
          int currentTotalUpdates = updateCount + callbackSends;
          int currentTotalFrames = successfulFrames + callbackSuccessful;
          int lastTotalUpdates = lastUpdateCount + lastCallbackSends;
          int lastTotalFrames = lastSuccessfulFrames + lastCallbackSuccessful;
          
          // Calculate rate over exactly 1 second
          updateRate = (currentTotalUpdates - lastTotalUpdates) * 1000.0 / (currentTime - lastFPSTime);
          frameRate = (currentTotalFrames - lastTotalFrames) * 1000.0 / (currentTime - lastFPSTime);
          
          lastFPSTime = currentTime;
          lastUpdateCount = updateCount;
          lastSuccessfulFrames = successfulFrames;
          lastCallbackSends = callbackSends;
          lastCallbackSuccessful = callbackSuccessful;
        }
        
        // Draw spinning line 
        float angle = (frameCount * 0.15); // Slightly faster spin than animation test
        int centerX = 25;
        int centerY = KYWY_DISPLAY_HEIGHT - 25;
        int lineX = centerX + cos(angle) * 25;
        int lineY = centerY + sin(angle) * 25;
        engine.display.drawLine((int16_t)centerX, (int16_t)centerY, (int16_t)lineX, (int16_t)lineY);
        
        // Display rates on screen
        engine.display.drawText(0, 10, String("UPS: " + String(updateRate, 1)).c_str());
        engine.display.drawText(0, 20, String("FPS: " + String(frameRate, 1)).c_str());
        engine.display.drawText(0, 30, String("Manual: " + String(successfulFrames)).c_str());
        engine.display.drawText(0, 40, String("Callback: " + String(callbackSuccessful)).c_str());
        
        // Show drop rate percentage
        float dropRate = totalAttempts > 0 ? ((totalAttempts - totalRealFrames) * 100.0) / totalAttempts : 0;
        engine.display.drawText(0, 50, String("Drop: " + String(dropRate, 1) + "%").c_str());
        
        // Show overall averages since FPS test phase started (using elapsed time correctly)
        float overallUpdateRate = totalAttempts * 1000.0 / elapsed;
        float overallFrameRate = totalRealFrames * 1000.0 / elapsed;
        engine.display.drawText(0, 60, String("Avg UPS: " + String(overallUpdateRate, 1)).c_str());
        engine.display.drawText(0, 70, String("Avg FPS: " + String(overallFrameRate, 1)).c_str());
        
        // Animated sprite at bottom - cycles through frames based on time
        int animFrame = (currentTime / MY_ANIMATION_SPEED) % MY_ANIMATION_FRAME_COUNT;
        int animX = KYWY_DISPLAY_WIDTH - MY_ANIMATION_WIDTH - 2; // Bottom right corner
        int animY = KYWY_DISPLAY_HEIGHT - MY_ANIMATION_HEIGHT - 2;
        engine.display.drawBitmap(animX, animY, MY_ANIMATION_WIDTH, MY_ANIMATION_HEIGHT, 
                                 (uint8_t*)my_animation_frames[animFrame]);
        
        // Progress percentage as text
        int progressPercent = (elapsed * 100) / 60000;
        engine.display.drawText(0, 85, String("Progress: " + String(progressPercent) + "%").c_str());
        
        engine.display.update();
        updateCount++;
        
        // Check if the frame was actually sent
        if (engine.display.checkPendingUpdate()) {
          successfulFrames++;
        }
        
        frameCount++;

        // End test after 1 minute (longer test for better averaging)
        if (elapsed > 60000) {
          testPhase = 3;
          phaseStartTime = currentTime;
          
          // Now print all the final results
          Serial.println("3 complete - test finished");
          Serial.println("=== FINAL RESULTS ===");
          Serial.print("Direct update rate: ");
          Serial.print(updateCount * 1000.0 / elapsed);
          Serial.println(" updates/sec");
          Serial.print("Direct frame rate: ");
          Serial.print(successfulFrames * 1000.0 / elapsed);
          Serial.println(" frames/sec");
          Serial.print("Callback sends: ");
          Serial.println(callbackSuccessful);
          Serial.print("Total real frame rate: ");
          Serial.print(totalRealFrames * 1000.0 / elapsed);
          Serial.println(" frames/sec");
          Serial.print("Overall drop rate: ");
          Serial.print(((totalAttempts - totalRealFrames) * 100.0) / totalAttempts);
          Serial.println("%");
        }
        break;
      }
      
    case 3: // Test complete - restart cycle
      {
        engine.display.clear();
        engine.display.drawText(0, 0, String("Cycle " + String(testCycle) + " Complete!").c_str());
        engine.display.drawText(0, 10, "Check Serial Monitor");
        engine.display.drawText(0, 20, "for results");
        engine.display.drawText(0, 40, String("Next cycle in: " + String(3 - (elapsed / 1000))).c_str());
        engine.display.update();
        
        // Restart cycle after 3 seconds
        if (elapsed > 3000) {
          testCycle++;
          testPhase = 0; // Back to start
          phaseStartTime = currentTime;
          frameCount = 0;
          updateCount = 0;
          successfulFrames = 0;
          engine.display.resetPerformanceCounters(); // Reset callback counters for new cycle
          Serial.println("Restarting test cycle...");
          Serial.println("");
        }
        break;
      }
  }
}