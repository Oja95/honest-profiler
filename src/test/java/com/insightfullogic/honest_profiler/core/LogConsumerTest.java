/**
 * Copyright (c) 2014 Richard Warburton (richard.warburton@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 **/
package com.insightfullogic.honest_profiler.core;

import com.insightfullogic.honest_profiler.core.parser.EventListener;
import com.insightfullogic.honest_profiler.core.parser.Method;
import com.insightfullogic.honest_profiler.core.parser.StackFrame;
import com.insightfullogic.honest_profiler.core.parser.TraceStart;
import com.insightfullogic.honest_profiler.core.store.LogSaver;
import com.insightfullogic.lambdabehave.JunitSuiteRunner;
import org.junit.runner.RunWith;
import org.slf4j.Logger;

import static com.insightfullogic.lambdabehave.Suite.describe;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;

@RunWith(JunitSuiteRunner.class)
public class LogConsumerTest {{

    describe("Parsing and Consuming a log", it -> {
        final String PRINTSTREAM = "Ljava/io/PrintStream;";
        final String PRINT_STREAM_JAVA = "PrintStream.java";

        it.should("parse a basic log", expect -> {
            EventListener listener = mock(EventListener.class);
            LogSaver saver = mock(LogSaver.class);
            Logger logger = mock(Logger.class);
            LogConsumer consumer = new LogConsumer(logger, Util.log0(), new DataConsumer(logger, null, saver, listener), false);

            expect.that(consumer.run()).is(true);
            verify(listener).handle(new TraceStart(2, 5));

            expect.that(consumer.run()).is(true);
            verify(listener).handle(new StackFrame(52, 1));

            expect.that(consumer.run()).is(true);
            verify(listener).handle(new Method(1, PRINT_STREAM_JAVA, PRINTSTREAM, "printf"));

            expect.that(consumer.run()).is(true);
            verify(listener).handle(new StackFrame(42, 2));

            expect.that(consumer.run()).is(true);
            verify(listener).handle(new Method(2, PRINT_STREAM_JAVA, PRINTSTREAM, "append"));

            // Next record
            expect.that(consumer.run()).is(true);
            expect.that(consumer.run()).is(true);
            expect.that(consumer.run()).is(true);
            expect.that(consumer.run()).is(false);

            verify(listener).endOfLog();
        });
    });

}}