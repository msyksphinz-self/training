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

package ru.ispras.microtesk.model.vliw.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.vliw.PE;
import ru.ispras.microtesk.model.vliw.TempVars;

import static ru.ispras.microtesk.model.vliw.TypeDefs.*;

public final class vliw extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "vliw",
          vliw.class,
          null
          );
      addArgument("c1", channel1.INFO);
      addArgument("c2", channel2.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final IsaPrimitive c1 = (IsaPrimitive) getArgument("c1", args);
      final IsaPrimitive c2 = (IsaPrimitive) getArgument("c2", args);

      return new vliw(c1, c2);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final IsaPrimitive c1;
  public final IsaPrimitive c2;

  public vliw(final IsaPrimitive c1, final IsaPrimitive c2) {
    assert channel1.INFO.isSupported(c1);
    assert channel2.INFO.isSupported(c2);

    this.c1 = c1;
    this.c2 = c2;

    addArgument("c1", c1);
    addArgument("c2", c2);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s; %s;", c1.text(vars__), c2.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s%s", c1.image(vars__), c2.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    vars__.NIA.access().store(pe__.CIA.access().load().add(Data.valueOf(WORD, 0x8)));
    vars__.BRANCH.access().store(Data.valueOf(BIT, 0x0));
    pe__.GPR.access(0).store(Data.valueOf(WORD, 0x0));
    c1.execute(pe__, vars__);
    pe__.GPR.access(0).store(Data.valueOf(WORD, 0x0));
    c2.execute(pe__, vars__);
    if (vars__.BRANCH.access().load().equals(Data.valueOf(BIT, 0x0))) {
      pe__.CIA.access().store(pe__.CIA.access().load().add(Data.valueOf(WORD, 0x8)));
    } else {
      pe__.CIA.access().store(vars__.JMPADDR.access().load());
    }
  }
}