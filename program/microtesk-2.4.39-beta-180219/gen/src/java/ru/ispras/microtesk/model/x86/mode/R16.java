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

package ru.ispras.microtesk.model.x86.mode;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.memory.Location;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class R16 extends IsaPrimitive {
  public static final String NAME = "R16";

  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.MODE,
          "R16",
          R16.class,
          WORD
          );
      addArgument("i", INDEX);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate i = (Immediate) getArgument("i", args);
      return new R16(i);
    }
  }

  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate i;

  public R16(final Immediate i) {
    this.i = i;

    addArgument("i", i);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s", (i.access().load().equals(Data.valueOf(INDEX, 0x0)) ? "AX" : (i.access().load().equals(Data.valueOf(INDEX, 0x1)) ? "CX" : (i.access().load().equals(Data.valueOf(INDEX, 0x2)) ? "DX" : (i.access().load().equals(Data.valueOf(INDEX, 0x3)) ? "BX" : (i.access().load().equals(Data.valueOf(INDEX, 0x4)) ? "SP" : (i.access().load().equals(Data.valueOf(INDEX, 0x5)) ? "BP" : (i.access().load().equals(Data.valueOf(INDEX, 0x6)) ? "SI" : (i.access().load().equals(Data.valueOf(INDEX, 0x7)) ? "DI" : "AX")))))))));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s", (i.access().load().equals(Data.valueOf(INDEX, 0x0)) ? "000" : (i.access().load().equals(Data.valueOf(INDEX, 0x1)) ? "001" : (i.access().load().equals(Data.valueOf(INDEX, 0x2)) ? "010" : (i.access().load().equals(Data.valueOf(INDEX, 0x3)) ? "011" : (i.access().load().equals(Data.valueOf(INDEX, 0x4)) ? "100" : (i.access().load().equals(Data.valueOf(INDEX, 0x5)) ? "101" : (i.access().load().equals(Data.valueOf(INDEX, 0x6)) ? "110" : (i.access().load().equals(Data.valueOf(INDEX, 0x7)) ? "111" : "000")))))))));
  }

  @Override
  public Location access(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    return annotate(pe__.GPR.access(i.access().load()), vars__);
  }
}