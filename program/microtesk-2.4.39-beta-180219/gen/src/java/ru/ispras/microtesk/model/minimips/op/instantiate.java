/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.minimips.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.minimips.PE;
import ru.ispras.microtesk.model.minimips.TempVars;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.minimips.TypeDefs.*;

public final class instantiate extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "instantiate",
          instantiate.class,
          null
          );
      addArgument("pid", WORD);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate pid = (Immediate) getArgument("pid", args);

      return new instantiate(pid);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate pid;

  public instantiate(final Immediate pid) {
    assert WORD.equals(pid.getType());

    this.pid = pid;

    addArgument("pid", pid);
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    pe__.PID.access().store(pid.access().load());
  }
}